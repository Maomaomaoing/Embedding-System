/******************************************************************************
lsm9ds0 Beaglebone Library
Alex Fuhr, The Ohio State University
https://github.com/projectzen/Beaglebone-LSM9DS0
Implements the LSM9DS0 functions on Beaglebone Black
******************************************************************************/

#include "lsm9ds0.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;
#define MAX_BUS 64
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
#define PI 3.14159265

LSM9DS0 dof(1, LSM9DS0_G, LSM9DS0_XM);

LSM9DS0::LSM9DS0(int bus, unsigned char gAddr, unsigned char xmAddr)
{
	// interfaceMode will keep track of whether we're using SPI or I2C:
	I2CBus = bus;
	xmAddress = xmAddr;
	gAddress = gAddr;
}

unsigned short LSM9DS0::begin(gyro_scale gScl, accel_scale aScl, mag_scale mScl, 
						gyro_odr gODR, accel_odr aODR, mag_odr mODR)
{
	// Store the given scales in class variables. These scale variables
	// are used throughout to calculate the actual g's, DPS,and Gs's.
	gScale = gScl;
	aScale = aScl;
	mScale = mScl;
	
	// Once we have the scale values, we can calculate the resolution
	// of each sensor. That's what these functions are for. One for each sensor
	calcgRes(); // Calculate DPS / ADC tick, stored in gRes variable
	calcmRes(); // Calculate Gs / ADC tick, stored in mRes variable
	calcaRes(); // Calculate g / ADC tick, stored in aRes variable
	
	// Now, initialize our hardware interface.
	initI2C();					// Initialize I2C
	
	// To verify communication, we can read from the WHO_AM_I register of
	// each device. Store those in a variable so we can return them.
	unsigned char gTest = gReadByte(WHO_AM_I_G);		// Read the gyro WHO_AM_I
	unsigned char xmTest = xmReadByte(WHO_AM_I_XM);	// Read the accel/mag WHO_AM_I
	
	// Gyro initialization stuff:
	initGyro();	// This will "turn on" the gyro. Setting up interrupts, etc.
	setGyroODR(gODR); // Set the gyro output data rate and bandwidth.
	setGyroScale(gScale); // Set the gyro range
	
	// Accelerometer initialization stuff:
	initAccel(); // "Turn on" all axes of the accel. Set up interrupts, etc.
	setAccelODR(aODR); // Set the accel data rate.
	setAccelScale(aScale); // Set the accel range.
	
	// Magnetometer initialization stuff:
	initMag(); // "Turn on" all axes of the mag. Set up interrupts, etc.
	setMagODR(mODR); // Set the magnetometer output data rate.
	setMagScale(mScale); // Set the magnetometer's range.
	
	// Once everything is initialized, return the WHO_AM_I registers we read:
	return (xmTest << 8) | gTest;
}

void LSM9DS0::initGyro()
{

	gWriteByte(CTRL_REG1_G, 0x0F); // Normal mode, enable all axes

	gWriteByte(CTRL_REG2_G, 0x00); // Normal mode, high cutoff frequency

	gWriteByte(CTRL_REG3_G, 0x88); 

	gWriteByte(CTRL_REG4_G, 0x00); // Set scale to 245 dps

	gWriteByte(CTRL_REG5_G, 0x00);

	configGyroInt(0x2A, 0, 0, 0, 0); // Trigger interrupt when above 0 DPS...
}

void LSM9DS0::initAccel()
{

	xmWriteByte(CTRL_REG0_XM, 0x00);
								 	
	xmWriteByte(CTRL_REG1_XM, 0x57); // 100Hz data rate, x/y/z all enabled
												 
	xmWriteByte(CTRL_REG2_XM, 0x00); // Set scale to 2g

	xmWriteByte(CTRL_REG3_XM, 0x04); 
}

void LSM9DS0::initMag()
{	
							 
	xmWriteByte(CTRL_REG5_XM, 0x94); // Mag data rate - 100 Hz, enable temperature sensor
						 
	xmWriteByte(CTRL_REG6_XM, 0x00); // Mag scale to +/- 2GS

	xmWriteByte(CTRL_REG7_XM, 0x00); // Continuous conversion mode

	xmWriteByte(CTRL_REG4_XM, 0x04); // Magnetometer data ready on INT2_XM (0x08)

	xmWriteByte(INT_CTRL_REG_M, 0x09); // Enable interrupts for mag, active-low, push-pull
}

void LSM9DS0::calLSM9DS0(float * gbias, float * abias)
{  
  unsigned char data[6] = {0, 0, 0, 0, 0, 0};
  short gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
  int samples, ii;
  
  // First get gyro bias
  char c = gReadByte(CTRL_REG5_G);
  gWriteByte(CTRL_REG5_G, c | 0x40);         // Enable gyro FIFO  
  usleep(20000);                                 // Wait for change to take effect
  gWriteByte(FIFO_CTRL_REG_G, 0x20 | 0x1F);  // Enable gyro FIFO stream mode and set watermark at 32 samples
  sleep(1);  // delay 1000 milliseconds to collect FIFO samples
  
  samples = (gReadByte(FIFO_SRC_REG_G) & 0x1F); // Read number of stored samples

  for(ii = 0; ii < samples ; ii++) {            // Read the gyro data stored in the FIFO
    gReadBytes(OUT_X_L_G,  &data[0], 6);
    gyro_bias[0] += (((short)data[1] << 8) | data[0]);
    gyro_bias[1] += (((short)data[3] << 8) | data[2]);
    gyro_bias[2] += (((short)data[5] << 8) | data[4]);
  }  

  gyro_bias[0] /= samples; // average the data
  gyro_bias[1] /= samples; 
  gyro_bias[2] /= samples; 
  
  gbias[0] = (float)gyro_bias[0]*gRes;  // Properly scale the data to get deg/s
  gbias[1] = (float)gyro_bias[1]*gRes;
  gbias[2] = (float)gyro_bias[2]*gRes;
  
  c = gReadByte(CTRL_REG5_G);
  gWriteByte(CTRL_REG5_G, c & ~0x40);  // Disable gyro FIFO  
  usleep(20000);
  gWriteByte(FIFO_CTRL_REG_G, 0x00);   // Enable gyro bypass mode
  

  //  Now get the accelerometer biases
  c = xmReadByte(CTRL_REG0_XM);
  xmWriteByte(CTRL_REG0_XM, c | 0x40);      // Enable accelerometer FIFO  
  usleep(20000);                                // Wait for change to take effect
  xmWriteByte(FIFO_CTRL_REG, 0x20 | 0x1F);  // Enable accelerometer FIFO stream mode and set watermark at 32 samples
  sleep(1);  // delay 1000 milliseconds to collect FIFO samples

  samples = (xmReadByte(FIFO_SRC_REG) & 0x1F); // Read number of stored accelerometer samples

   for(ii = 0; ii < samples ; ii++) {          // Read the accelerometer data stored in the FIFO
    xmReadBytes(OUT_X_L_A, &data[0], 6);
    accel_bias[0] += (((short)data[1] << 8) | data[0]);
    accel_bias[1] += (((short)data[3] << 8) | data[2]);
    accel_bias[2] += (((short)data[5] << 8) | data[4]) - (short)(1./aRes); // Assumes sensor facing up!
  }  

  accel_bias[0] /= samples; // average the data
  accel_bias[1] /= samples; 
  accel_bias[2] /= samples; 
  
  abias[0] = (float)accel_bias[0]*aRes; // Properly scale data to get gs
  abias[1] = (float)accel_bias[1]*aRes;
  abias[2] = (float)accel_bias[2]*aRes;

  c = xmReadByte(CTRL_REG0_XM);
  xmWriteByte(CTRL_REG0_XM, c & ~0x40);    // Disable accelerometer FIFO  
  usleep(20000);
  xmWriteByte(FIFO_CTRL_REG, 0x00);       // Enable accelerometer bypass mode
}

void LSM9DS0::readAccel()
{
	unsigned char temp[6]; // We'll read six chars from the accelerometer into temp	
	xmReadBytes(OUT_X_L_A, temp, 6); // Read 6 chars, beginning at OUT_X_L_A
	ax = (temp[1] << 8) | temp[0]; // Store x-axis values into ax
	ay = (temp[3] << 8) | temp[2]; // Store y-axis values into ay
	az = (temp[5] << 8) | temp[4]; // Store z-axis values into az
}

void LSM9DS0::readMag()
{
	unsigned char temp[6]; // We'll read six chars from the mag into temp	
	xmReadBytes(OUT_X_L_M, temp, 6); // Read 6 chars, beginning at OUT_X_L_M
	mx = (temp[1] << 8) | temp[0]; // Store x-axis values into mx
	my = (temp[3] << 8) | temp[2]; // Store y-axis values into my
	mz = (temp[5] << 8) | temp[4]; // Store z-axis values into mz
}

void LSM9DS0::readTemp()
{
	unsigned char temp[2]; // We'll read two chars from the temperature sensor into temp	
	xmReadBytes(OUT_TEMP_L_XM, temp, 2); // Read 2 chars, beginning at OUT_TEMP_L_M
	temperature = (((short) temp[1] << 12) | temp[0] << 4 ) >> 4; // Temperature is a 12-bit signed integer
}

void LSM9DS0::readGyro()
{
	unsigned char temp[6]; // We'll read six chars from the gyro into temp
	gReadBytes(OUT_X_L_G, temp, 6); // Read 6 chars, beginning at OUT_X_L_G
	gx = (temp[1] << 8) | temp[0]; // Store x-axis values into gx
	gy = (temp[3] << 8) | temp[2]; // Store y-axis values into gy
	gz = (temp[5] << 8) | temp[4]; // Store z-axis values into gz
}

float LSM9DS0::calcGyro(short gyro)
{
	// Return the gyro raw reading times our pre-calculated DPS / (ADC tick):
	return gRes * gyro; 
}

float LSM9DS0::calcAccel(short accel)
{
	// Return the accel raw reading times our pre-calculated g's / (ADC tick):
	return aRes * accel;
}

float LSM9DS0::calcMag(short mag)
{
	// Return the mag raw reading times our pre-calculated Gs / (ADC tick):
	return mRes * mag;
}

void LSM9DS0::setGyroScale(gyro_scale gScl)
{
	// We need to preserve the other chars in CTRL_REG4_G. So, first read it:
	unsigned char temp = gReadByte(CTRL_REG4_G);
	// Then mask out the gyro scale bits:
	temp &= 0xFF^(0x3 << 4);
	// Then shift in our new scale bits:
	temp |= gScl << 4;
	// And write the new register value back into CTRL_REG4_G:
	gWriteByte(CTRL_REG4_G, temp);
	
	// We've updated the sensor, but we also need to update our class variables
	// First update gScale:
	gScale = gScl;
	// Then calculate a new gRes, which relies on gScale being set correctly:
	calcgRes();
}

void LSM9DS0::setAccelScale(accel_scale aScl)
{
	// We need to preserve the other chars in CTRL_REG2_XM. So, first read it:
	unsigned char temp = xmReadByte(CTRL_REG2_XM);
	// Then mask out the accel scale bits:
	temp &= 0xFF^(0x3 << 3);
	// Then shift in our new scale bits:
	temp |= aScl << 3;
	// And write the new register value back into CTRL_REG2_XM:
	xmWriteByte(CTRL_REG2_XM, temp);
	
	// We've updated the sensor, but we also need to update our class variables
	// First update aScale:
	aScale = aScl;
	// Then calculate a new aRes, which relies on aScale being set correctly:
	calcaRes();
}

void LSM9DS0::setMagScale(mag_scale mScl)
{
	// We need to preserve the other chars in CTRL_REG6_XM. So, first read it:
	unsigned char temp = xmReadByte(CTRL_REG6_XM);
	// Then mask out the mag scale bits:
	temp &= 0xFF^(0x3 << 5);
	// Then shift in our new scale bits:
	temp |= mScl << 5;
	// And write the new register value back into CTRL_REG6_XM:
	xmWriteByte(CTRL_REG6_XM, temp);
	
	// We've updated the sensor, but we also need to update our class variables
	// First update mScale:
	mScale = mScl;
	// Then calculate a new mRes, which relies on mScale being set correctly:
	calcmRes();
}

void LSM9DS0::setGyroODR(gyro_odr gRate)
{
	// We need to preserve the other chars in CTRL_REG1_G. So, first read it:
	unsigned char temp = gReadByte(CTRL_REG1_G);
	// Then mask out the gyro ODR bits:
	temp &= 0xFF^(0xF << 4);
	// Then shift in our new ODR bits:
	temp |= (gRate << 4);
	// And write the new register value back into CTRL_REG1_G:
	gWriteByte(CTRL_REG1_G, temp);
}
void LSM9DS0::setAccelODR(accel_odr aRate)
{
	// We need to preserve the other chars in CTRL_REG1_XM. So, first read it:
	unsigned char temp = xmReadByte(CTRL_REG1_XM);
	// Then mask out the accel ODR bits:
	temp &= 0xFF^(0xF << 4);
	// Then shift in our new ODR bits:
	temp |= (aRate << 4);
	// And write the new register value back into CTRL_REG1_XM:
	xmWriteByte(CTRL_REG1_XM, temp);
}
void LSM9DS0::setAccelABW(accel_abw abwRate)
{
	// We need to preserve the other chars in CTRL_REG2_XM. So, first read it:
	unsigned char temp = xmReadByte(CTRL_REG2_XM);
	// Then mask out the accel ABW bits:
	temp &= 0xFF^(0x3 << 7);
	// Then shift in our new ODR bits:
	temp |= (abwRate << 7);
	// And write the new register value back into CTRL_REG2_XM:
	xmWriteByte(CTRL_REG2_XM, temp);
}
void LSM9DS0::setMagODR(mag_odr mRate)
{
	// We need to preserve the other chars in CTRL_REG5_XM. So, first read it:
	unsigned char temp = xmReadByte(CTRL_REG5_XM);
	// Then mask out the mag ODR bits:
	temp &= 0xFF^(0x7 << 2);
	// Then shift in our new ODR bits:
	temp |= (mRate << 2);
	// And write the new register value back into CTRL_REG5_XM:
	xmWriteByte(CTRL_REG5_XM, temp);
}

void LSM9DS0::configGyroInt(unsigned char int1Cfg, unsigned short int1ThsX, unsigned short int1ThsY, unsigned short int1ThsZ, unsigned char duration)
{
	gWriteByte(INT1_CFG_G, int1Cfg);
	gWriteByte(INT1_THS_XH_G, (int1ThsX & 0xFF00) >> 8);
	gWriteByte(INT1_THS_XL_G, (int1ThsX & 0xFF));
	gWriteByte(INT1_THS_YH_G, (int1ThsY & 0xFF00) >> 8);
	gWriteByte(INT1_THS_YL_G, (int1ThsY & 0xFF));
	gWriteByte(INT1_THS_ZH_G, (int1ThsZ & 0xFF00) >> 8);
	gWriteByte(INT1_THS_ZL_G, (int1ThsZ & 0xFF));
	if (duration)
		gWriteByte(INT1_DURATION_G, 0x80 | duration);
	else
		gWriteByte(INT1_DURATION_G, 0x00);
}

void LSM9DS0::calcgRes()
{

	switch (gScale)
	{
	case G_SCALE_245DPS:
		gRes = 245.0 / 32768.0;
		break;
	case G_SCALE_500DPS:
		gRes = 500.0 / 32768.0;
		break;
	case G_SCALE_2000DPS:
		gRes = 2000.0 / 32768.0;
		break;
	}
}

void LSM9DS0::calcaRes()
{
	aRes = aScale == A_SCALE_16G ? 16.0 / 32768.0 : 
		   (((float) aScale + 1.0) * 2.0) / 32768.0;
}

void LSM9DS0::calcmRes()
{
	mRes = mScale == M_SCALE_2GS ? 2.0 / 32768.0 : 
	       (float) (mScale << 2) / 32768.0;
}
	
void LSM9DS0::gWriteByte(unsigned char subAddress, unsigned char data)
{
	I2CwriteByte(gAddress, subAddress, data);
}

void LSM9DS0::xmWriteByte(unsigned char subAddress, unsigned char data)
{
	return I2CwriteByte(xmAddress, subAddress, data);
}

unsigned char LSM9DS0::gReadByte(unsigned char subAddress)
{
	return I2CreadByte(gAddress, subAddress);
}

void LSM9DS0::gReadBytes(unsigned char subAddress, unsigned char * dest, unsigned char count)
{
	I2CreadBytes(gAddress, subAddress, dest, count);
}

unsigned char LSM9DS0::xmReadByte(unsigned char subAddress)
{
	return I2CreadByte(xmAddress, subAddress);
}

void LSM9DS0::xmReadBytes(unsigned char subAddress, unsigned char * dest, unsigned char count)
{
	I2CreadBytes(xmAddress, subAddress, dest, count);
}

void LSM9DS0::initI2C()
{
	//Open I2C file as bidirectional
	char namebuf[MAX_BUS];
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
	if ((file=open(namebuf, O_RDWR)) < 0) {
		cout << "Failed to open sensor on " << namebuf << " I2C Bus" << endl;
	}
}

void LSM9DS0::I2CwriteByte(unsigned char address, unsigned char subAddress, unsigned char data)
{
	if (ioctl(file, I2C_SLAVE, address) < 0) {
		cout << "I2C_SLAVE address " << address << " failed..." << endl;
	}
	int result = i2c_smbus_write_byte_data(file, subAddress, data);
	if (result == -1) {
		printf("Failed to write byte to " + subAddress);
	}
}

unsigned char LSM9DS0::I2CreadByte(unsigned char address, unsigned char subAddress)
{
	if (ioctl(file, I2C_SLAVE, address) < 0) {
		cout << "I2C_SLAVE address " << address << " failed..." << endl;
	}
	return i2c_smbus_read_byte_data(file, subAddress);
}

void LSM9DS0::I2CreadBytes(unsigned char address, unsigned char subAddress, unsigned char * dest, unsigned char count)
{  
	if (ioctl(file, I2C_SLAVE, address) < 0) {
		cout << "I2C_SLAVE address " << address << " failed..." << endl;
	}
	unsigned char sAddr = subAddress | 0x80; // or with 0x80 to indicate multi-read
	int result = i2c_smbus_read_i2c_block_data(file, sAddr, count, dest);
	if (result != count) {
		printf("Failed to read block from I2C");
	}
}
