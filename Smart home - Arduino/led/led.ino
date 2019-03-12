#include <convert.h>


#include <stdio.h>
#define greenPin 6
#define redPin 5
#define bluePin 3

byte prefix;
byte notUse;
char letter1, letter2;
int i;
int color[3];
int charge=12;
convert CONVERT;

void setup(){
    Serial.begin(9600);
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    //Serial.println("Enter prefix and GRB color");
    pinMode(charge, OUTPUT);
}
void loop(){
    digitalWrite(charge,HIGH);  
    if(Serial.available()){
        
      prefix=Serial.read();
      if(prefix=='F'){
        
          while(Serial.available()<2){}
          
          for(i=0;i<2;i++)
          {
          notUse=Serial.read();
          }
          
        }
      else if(prefix=='E'){
          for(i=0;i<3;i++){
              while(Serial.available()<2){}
              letter1=Serial.read();
              letter2=Serial.read();
              color[i]=CONVERT.charToInt(letter1,letter2);
              }
          analogWrite(greenPin, color[0]);
          analogWrite(redPin, color[1]);
          analogWrite(bluePin, color[2]);
          }
      }//end if
}//end loop

