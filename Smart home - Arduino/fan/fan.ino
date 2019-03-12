#include<stdio.h>
#define pwm_pin 11 //blue
#define sense_pin 10 //green
int charge=12;
byte noUse, prefix;

void setup(){
  Serial.begin(9600);
  pinMode(pwm_pin,OUTPUT);
  pinMode(charge, OUTPUT);
}

void loop(){
  
  int n,c1,c2,i;
digitalWrite(charge,HIGH);
     if(Serial.available())
     {
       prefix = Serial.read();
       
       if(prefix=='E')
       {
         while(Serial.available()<6){}
         for(i=0;i<6;i++)
         {
           
           noUse = Serial.read();
         }
       }  
       else if(prefix=='F')
       {
          while(Serial.available()<2){}
          c1=Serial.read();
          c2=Serial.read();
     
          c1=convert(c1);
          c2=convert(c2);
          n=c1*16+c2;
     

          analogWrite(pwm_pin,n);   
       }//end else if       
     }//end if
}//end loop

int convert(int n){

    if(n<58&&n>47)
      return n-48;
    else if(n<71&&n>64)
      return n-55;
    else if(n<103&&n>96)
      return n-87;
    else return 0;
}

