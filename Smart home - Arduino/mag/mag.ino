int magnet=2; //connect tilt to D2
int charge=12;
int LED=13;
int change=1;
char n;
void setup() {
  
  pinMode(LED, OUTPUT);
  pinMode(magnet, INPUT);
  pinMode(charge, OUTPUT);
  digitalWrite(charge,HIGH);
  Serial.begin(9600);
}

void loop() {

  
  
if(digitalRead(magnet)==HIGH) //when the digital output value of tilt is high, turn on the LED.
{
	digitalWrite(LED, HIGH);
        n=62;//open:>
        if(change==0){
          Serial.print(n);}
        change=1;}
if(digitalRead(magnet)==LOW) //when the digital output value of tilt is low, turn off the LED.
{
	digitalWrite(LED, LOW);
        n=63;//close:?
        if(change==1){
          Serial.print(n);}
        change=0;} 
}
