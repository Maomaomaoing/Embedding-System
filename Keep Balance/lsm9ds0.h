/******************************************************************************
lsm9ds0.h
LSM9DS0 library for Beaglebone Black
Alex Fuhr, The Ohio State University
Original Creation Date: February 14, 2014 (Happy Valentines Day!)
https://github.com/projectzen/Beaglebone-LSM9DS0
******************************************************************************/
//#ifndef __lsm9ds0.h
//#define __lsm9ds0.h


////////////////////////////
// LSM9DS0 Gyro Registers //
////////////////////////////
#define WHO_AM_I_G			0x0F
#define CTRL_REG1_G			0x20
#define CTRL_REG2_G			0x21
#define CTRL_REG3_G			0x22
#define CTRL_REG4_G			0x23
#define CTRL_REG5_G			0x24
#define REFERENCE_G			0x25
#define STATUS_REG_G		0x27
#define OUT_X_L_G			0x28
#define OUT_X_H_G			0x29
#define OUT_Y_L_G			0x2A
#define OUT_Y_H_G			0x2B
#define OUT_Z_L_G			0x2C
#define OUT_Z_H_G			0x2D
#define FIFO_CTRL_REG_G		0x2E
#define FIFO_SRC_REG_G		0x2F
#define INT1_CFG_G			0x30
#define INT1_SRC_G			0x31
#define INT1_THS_XH_G		0x32
#define INT1_THS_XL_G		0x33
#define INT1_THS_YH_G		0x34
#define INT1_THS_YL_G		0x35
#define INT1_THS_ZH_G		0x36
#define INT1_THS_ZL_G		0x37
#define INT1_DURATION_G		0x38

//////////////////////////////////////////
// LSM9DS0 Accel/Magneto (XM) Registers //
//////////////////////////////////////////
#define OUT_TEMP_L_XM		0x05
#define OUT_TEMP_H_XM		0x06
#define STATUS_REG_M		0x07
#define OUT_X_L_M			0x08
#define OUT_X_H_M			0x09
#define OUT_Y_L_M			0x0A
#define OUT_Y_H_M			0x0B
#define OUT_Z_L_M			0x0C
#define OUT_Z_H_M			0x0D
#define WHO_AM_I_XM			0x0F
#define INT_CTRL_REG_M		0x12
#define INT_SRC_REG_M		0x13
#define INT_THS_L_M			0x14
#define INT_THS_H_M			0x15
#define OFFSET_X_L_M		0x16
#define OFFSET_X_H_M		0x17
#define OFFSET_Y_L_M		0x18
#define OFFSET_Y_H_M		0x19
#define OFFSET_Z_L_M		0x1A
#define OFFSET_Z_H_M		0x1B
#define REFERENCE_X			0x1C
#define REFERENCE_Y			0x1D
#define REFERENCE_Z			0x1E
#define CTRL_REG0_XM		0x1F
#define CTRL_REG1_XM		0x20
#define CTRL_REG2_XM		0x21
#define CTRL_REG3_XM		0x22
#define CTRL_REG4_XM		0x23
#define CTRL_REG5_XM		0x24
#define CTRL_REG6_XM		0x25
#define CTRL_REG7_XM		0x26
#define STATUS_REG_A		0x27
#define OUT_X_L_A			0x28
#define OUT_X_H_A			0x29
#define OUT_Y_L_A			0x2A
#define OUT_Y_H_A			0x2B
#define OUT_Z_L_A			0x2C
#define OUT_Z_H_A			0x2D
#define FIFO_CTRL_REG		0x2E
#define FIFO_SRC_REG		0x2F
#define INT_GEN_1_REG		0x30
#define INT_GEN_1_SRC		0x31
#define INT_GEN_1_THS		0x32
#define INT_GEN_1_DURATION	0x33
#define INT_GEN_2_REG		0x34
#define INT_GEN_2_SRC		0x35
#define INT_GEN_2_THS		0x36
#define INT_GEN_2_DURATION	0x37
#define CLICK_CFG			0x38
#define CLICK_SRC			0x39
#define CLICK_THS			0x3A
#define TIME_LIMIT			0x3B
#define TIME_LATENCY		0x3C
#define TIME_WINDOW			0x3D
#define ACT_THS				0x3E
#define ACT_DUR				0x3F

// The LSM9DS0 functions over both I2C or SPI. This library supports both.
// But the interface mode used must be sent to the LSM9DS0 constructor. Use
// one of these two as the first parameter of the constructor.
enum interface_mode
{
	MODE_SPI,
	MODE_I2C,
};

class LSM9DS0
{
public:
	// gyro_scale defines the possible full-scale ranges of the gyroscope:
	enum gyro_scale
	{
		G_SCALE_245DPS,		// 00:  245 degrees per second
		G_SCALE_500DPS,		// 01:  500 dps
		G_SCALE_2000DPS,	// 10:  2000 dps
	};
	// accel_scale defines all possible FSR's of the accelerometer:
	enum accel_scale
	{
		A_SCALE_2G,	// 000:  2g
		A_SCALE_4G,	// 001:  4g
		A_SCALE_6G,	// 010:  6g
		A_SCALE_8G,	// 011:  8g
		A_SCALE_16G	// 100:  16g
	};
	// mag_scale defines all possible FSR's of the magnetometer:
	enum mag_scale
	{
		M_SCALE_2GS,	// 00:  2Gs
		M_SCALE_4GS, 	// 01:  4Gs
		M_SCALE_8GS,	// 10:  8Gs
		M_SCALE_12GS,	// 11:  12Gs
	};
	// gyro_odr defines all possible data rate/bandwidth combos of the gyro:
	enum gyro_odr
	{							// ODR (Hz) --- Cutoff
		G_ODR_95_BW_125  = 0x0, //   95         12.5
		G_ODR_95_BW_25   = 0x1, //   95          25
		// 0x2 and 0x3 define the same data rate and bandwidth
		G_ODR_190_BW_125 = 0x4, //   190        12.5
		G_ODR_190_BW_25  = 0x5, //   190         25
		G_ODR_190_BW_50  = 0x6, //   190         50
		G_ODR_190_BW_70  = 0x7, //   190         70
		G_ODR_380_BW_20  = 0x8, //   380         20
		G_ODR_380_BW_25  = 0x9, //   380         25
		G_ODR_380_BW_50  = 0xA, //   380         50
		G_ODR_380_BW_100 = 0xB, //   380         100
		G_ODR_760_BW_30  = 0xC, //   760         30
		G_ODR_760_BW_35  = 0xD, //   760         35
		G_ODR_760_BW_50  = 0xE, //   760         50
		G_ODR_760_BW_100 = 0xF, //   760         100
	};
	// accel_oder defines all possible output data rates of the accelerometer:
	enum accel_odr
	{
		A_POWER_DOWN, 	// Power-down mode (0x0)
		A_ODR_3125,		// 3.125 Hz	(0x1)
		A_ODR_625,		// 6.25 Hz (0x2)
		A_ODR_125,		// 12.5 Hz (0x3)
		A_ODR_25,		// 25 Hz (0x4)
		A_ODR_50,		// 50 Hz (0x5)
		A_ODR_100,		// 100 Hz (0x6)
		A_ODR_200,		// 200 Hz (0x7)
		A_ODR_400,		// 400 Hz (0x8)
		A_ODR_800,		// 800 Hz (9)
		A_ODR_1600		// 1600 Hz (0xA)
	};

      // accel_abw defines all possible anti-aliasing filter rates of the accelerometer:
	enum accel_abw
	{
		A_ABW_773,		// 773 Hz (0x0)
		A_ABW_194,		// 194 Hz (0x1)
		A_ABW_362,		// 362 Hz (0x2)
		A_ABW_50,		//  50 Hz (0x3)
	};


	// mag_oder defines all possible output data rates of the magnetometer:
	enum mag_odr
	{
		M_ODR_3125,	// 3.125 Hz (0x00)
		M_ODR_625,	// 6.25 Hz (0x01)
		M_ODR_125,	// 12.5 Hz (0x02)
		M_ODR_25,	// 25 Hz (0x03)
		M_ODR_50,	// 50 (0x04)
		M_ODR_100,	// 100 Hz (0x05)
	};

	short gx, gy, gz; // x, y, and z axis readings of the gyroscope
	short ax, ay, az; // x, y, and z axis readings of the accelerometer
	short mx, my, mz; // x, y, and z axis readings of the magnetometer
        short temperature;
	float abias[3];
        float gbias[3];

	LSM9DS0(int bus, unsigned char gAddr, unsigned char xmAddr);

	unsigned short begin(gyro_scale gScl = G_SCALE_245DPS, 
				accel_scale aScl = A_SCALE_2G, mag_scale mScl = M_SCALE_2GS,
				gyro_odr gODR = G_ODR_95_BW_125, accel_odr aODR = A_ODR_50, 
				mag_odr mODR = M_ODR_50);

	void readGyro();

	void readAccel();

	void readMag();

	void readTemp();

	float calcGyro(short gyro);

	float calcAccel(short accel);

	float calcMag(short mag);

	void setGyroScale(gyro_scale gScl);

	void setAccelScale(accel_scale aScl);

	void setMagScale(mag_scale mScl);

	void setGyroODR(gyro_odr gRate);

	void setAccelODR(accel_odr aRate); 	

	void setAccelABW(accel_abw abwRate);

	void setMagODR(mag_odr mRate);

	void configGyroInt(unsigned char int1Cfg, unsigned short int1ThsX = 0,
						  unsigned short int1ThsY = 0, unsigned short int1ThsZ = 0, 
						  unsigned char duration = 0);


        void calLSM9DS0(float gbias[3], float abias[3]);


private:	

	unsigned char xmAddress, gAddress;

	int I2CBus;

	int file;

	gyro_scale gScale;
	accel_scale aScale;
	mag_scale mScale;

	float gRes, aRes, mRes;

	void initGyro();

	void initAccel();

	void initMag();

	unsigned char gReadByte(unsigned char subAddress);

	void gReadBytes(unsigned char subAddress, unsigned char * dest, unsigned char count);

	void gWriteByte(unsigned char subAddress, unsigned char data);

	unsigned char xmReadByte(unsigned char subAddress);

	void xmReadBytes(unsigned char subAddress, unsigned char * dest, unsigned char count);

	void xmWriteByte(unsigned char subAddress, unsigned char data);

	void calcgRes();

	void calcmRes();

	void calcaRes();

	void initSPI();

	void SPIwriteByte(unsigned char csPin, unsigned char subAddress, unsigned char data);
	
	unsigned char SPIreadByte(unsigned char csPin, unsigned char subAddress);
	
	void SPIreadBytes(unsigned char csPin, unsigned char subAddress, 
							unsigned char * dest, unsigned char count);

	void initI2C();

	void I2CwriteByte(unsigned char address, unsigned char subAddress, unsigned char data);

	unsigned char I2CreadByte(unsigned char address, unsigned char subAddress);

	void I2CreadBytes(unsigned char address, unsigned char subAddress, unsigned char * dest, unsigned char count);
};

//#endif // SFE_LSM9DS0_H //
