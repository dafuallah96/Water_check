#include <SoftwareSerial.h>
SoftwareSerial SMS(9, 10);
#define trigpin 7  
#define echopin 6
// rx(9) and tx(10)
                          // rx(9-arduino) connect to tx(gsm module)
                          // tx(10-arduino) connect to rx(gsm module)
void setup()
{
  SMS.begin(9600);       // baud rate for gsm module  
  Serial.begin(9600); 
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);// baud rate for serial monitor 
  delay(100);
  
  pinMode(12, INPUT); 
}
void loop()
{
  int duration, distance;
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(1000);  
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin,HIGH);
  distance = ( duration / 2) / 29.1;
  Serial.println("cm:"); 
  Serial.println(distance);

  
  if(  (distance > 0) && (distance <= 10)   ) 
  {
    sendmessage();
    //recievemessage(); // to receive message enable receivemessage function 
                         // and "//" the sendmessage function
  }
   if (SMS.available()>0){// verify any bits which income through pins 9 and 10
   Serial.write(SMS.read());// write income data on serial monitor
 }
}
 void sendmessage()
{

  SMS.println("AT");
  updateSerial();
  
  SMS.println("AT+CMGF=1"); //AT command for text mode of gsm module 
  updateSerial();  
  SMS.println("AT+CMGS=\"+60102062026\""); //  x is your mobile number
  updateSerial();
  SMS.println("Water can be drank now!");// the text you want to send
  updateSerial();
  SMS.write(26);// ASCII code of CTRL+Z
  delay(10000);
  Serial.println("Sent"); 
}
 void recievemessage()
{
  SMS.println("AT+CNMI=2,2,0,0,0"); // AT command to receive SMS
  delay(1000);
 }


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    SMS.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(SMS.available()) 
  {
    Serial.write(SMS.read());//Forward what Software Serial received to Serial Port
  }
}
