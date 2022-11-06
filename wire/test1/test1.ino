#include <Wire.h>

#define SLAVE_ADDRESS 0x0f

byte data_to_echo = 0;
int smoke1 = A3;
int smoke2 = A2;
int smoke3 = A1;
int smoke4 = A0;
int analogSensor1 = 0;
int analogSensor2 = 0;
byte response[5]; // this data is sent to the Pi
int threshold255 = 90;
int threshold1023 = 380;
int threshold255_2 = 180;
int threshold1023_2 = 760;
void setup() 
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  pinMode(smoke1,INPUT);
  pinMode(smoke2,INPUT);
}

void loop() {
  //loop code here
}

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++) {
    data_to_echo = Wire.read();
  }
}

void sendData()
{
  int v1=analogRead(smoke1);
  int v2=analogRead(smoke2);
  int v3=analogRead(smoke3);
  int v4=analogRead(smoke4);
  // Arduino returns 10bit data but we need to convert it to 8bit 
  v1=map(v1,0,1023,0,255);
  v2=map(v2,0,1023,0,255);
  v3=map(v3,0,1023,0,255);
  v4=map(v4,0,1023,0,255);
  response[0]=(byte)v1;
  response[1]=(byte)v2;
  response[2]=(byte)v3;
  response[3]=(byte)v4;
  Wire.write(response,5); // return data to PI
}
