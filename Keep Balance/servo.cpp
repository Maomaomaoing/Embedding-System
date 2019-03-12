#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
//#include "lsm9ds0.h"
#include "lsm9ds0.cpp"
using namespace std;

#define ADC_DIR "/sys/bus/iio/devices/"
#define PWM_DIR "/sys/bus/platform/drivers/"
#define PI 3.14159265

FILE *stream1=NULL;
FILE *stream2=NULL;

void servo_init();
void servo_ctrl(float degree);
void accel_init();
float read_degree();
void LED_ctrl(int num);

LSM9DS0 dof(1, LSM9DS0_G, LSM9DS0_XM);

int main()
{ 
    float c_degree=0;    
    float degree;
    int num;
    
    /* initial LED, servo motor and LSM9DS0*/
    stream1 = fopen ("/sys/class/gpio/export", "w");
    fwrite ("66", 1, 22, stream1);
    fclose(stream1);   
    servo_init();
    cout<<"Servo initialize success"<<endl;
    accel_init();
    cout<<"Accelerometer initialize success"<<endl;
    
    /* read current degree */
    c_degree=read_degree();
    
    while(1)
	{
		/*read change degree*/
		degree = read_degree();
		
		/*control LED, horizontal: dark, else light*/
		if(degree>10||degree<-10)
		    LED_ctrl(1);
		else
		    LED_ctrl(0);
		
		/*rotate to balance(0 degree)*/    
		c_degree-=degree;
		servo_ctrl(c_degree);
		
		usleep(500000);
    }
}

void servo_init()
{
    stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/run","w+");

    if (stream2==NULL)     /*假如沒有文件就自行加入PWM*/
    {
        stream2=fopen("/sys/devices/bone_capemgr.9/slots","w");
        fwrite("bone_pwm_P9_14",sizeof(int),14,stream2);
        fclose(stream2);

        stream2=fopen("/sys/devices/bone_capemgr.9/slots","w");
        fwrite("am33xx_pwm",sizeof(int),10,stream2);   
        fclose(stream2);

        stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/run","w+");
    }

    fwrite("0",sizeof(int),1,stream2);//set run=0
    fclose(stream2);

    stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/period","w+");//set period = 20000000
    fwrite("20000000",sizeof(int),8,stream2); 
    fclose(stream2);

    stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/duty","w+");//set duty = 18530000 (degree=0)
    fwrite("18530000",sizeof(int),8,stream2); 
    fclose(stream2);

    stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/run","w+");
    fwrite("1",sizeof(int),1,stream2);//set run=1
    fclose(stream2);

    usleep (1000);
}

void servo_ctrl(float degree)
{
	int duty= 18530000;
	char angle[8];
	
	/*check input degree*/
	if(degree>90||degree<-90)
	{
		printf("degree must between -90 ~ 90\n");
	}
	else
	{
		/*transform degree to duty, and duty must between 17600000 ~ 19460000 */
		duty=(int)(degree+90)*1860000/180+17600000;
		duty=duty>19460000?19460000:duty;
		duty=duty<17600000?17600000:duty;

		sprintf(angle,"%d",duty);// integer to string

		stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/run","w+");
		fwrite("0",sizeof(int),1,stream2);//set run=0
		fclose(stream2);

		stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/duty","w+");
		fwrite(angle,sizeof(int),8,stream2); //set duty 
		fclose(stream2);

		stream2=fopen(PWM_DIR"pwm_test/pwm_test_P9_14.15/run","w+");
		fwrite("1",sizeof(int),1,stream2);//set run=1
		fclose(stream2);
	}
}

void accel_init()
{
	int status = dof.begin();
	cout << "Status: " << status << endl;
} 

float read_degree()
{
	float num,angle;
	
	/* read accel*/
	dof.readAccel();
	
	/*use accel z-axis to decide degree*/
	num = dof.calcAccel(dof.az);
    num = num>1?1:num; //maximum is 1G
    angle = acos(num) * 180.0 / PI; //always>0
	
	/*decide tilt direction*/
    if(dof.calcAccel(dof.ay)<0)
		angle=-angle;
    
    cout<<"angle : "<<angle<<endl;

    return angle;
}
void LED_ctrl(int num)
{
	stream1 = fopen ("/sys/class/gpio/gpio66/direction", "w");
	if(num==1)
		fwrite ("high", 1, 4, stream1);   //set the pin to HIGH
	else
		fwrite ("low", 1, 3, stream1);   //set the pin to LOW
	fclose (stream1);
}
