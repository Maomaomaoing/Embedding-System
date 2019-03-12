#include <dht.h>
#include <stdio.h>

#define dht_dpin 13 //定義訊號要從Pin 2 進來  
#define charge 12
dht DHT;   
void setup(){   
  Serial.begin(9600);     
  pinMode(charge, OUTPUT);
  
  delay(1000);             //Wait rest of 1000ms recommended delay before  
}
  
void loop(){   
  char i;
  digitalWrite(charge,HIGH);
  DHT.read11(dht_dpin);   //去library裡面找DHT.read11  
  //Serial.print(DHT.temperature);
  i=64+DHT.temperature;
  Serial.print(i);
  delay(3000);            //每2000ms更新一次   
}

