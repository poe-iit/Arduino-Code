#include <Wire.h>
#include <Adafruit_MCP3008.h>
#include <DHT.h>

#define SLAVE_ADDRESS 0x10
byte receivedData = 0;
byte response[17]; //this is the byte array we will send to the Pi over I2C
Adafruit_MCP3008 adc; 
int smoke1 = A3;
int smoke2 = A2;
int smoke3 = A1;
int smoke4 = A0;
int f1 = 9;
int f2 = 8;
int f3 = 7;
int f4 = 6;
int b1 = 5;
int b2 = 4;
int b3 = 3;
int b4 = 2;
int h1 = 1;
int h2 = 0;

DHT dht1(h1,DHT22);
DHT dht2(h2,DHT22);

int smokesens1;
int smokesens2;
int smokesens3;
int smokesens4;
int flame1;
int flame2;
int flame3;
int flame4;
int temp1; 
int temp2;
int temp3;
int temp4;
int hum1;
int hum2;
int water1;
int water2;

void setup() {
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  pinMode(smoke1, INPUT);
  pinMode(smoke2, INPUT);
  pinMode(smoke3, INPUT);
  pinMode(smoke4, INPUT);
  pinMode(f1, INPUT);
  pinMode(f2, INPUT);
  pinMode(f3, INPUT);
  pinMode(f4, INPUT);
  pinMode(h1, INPUT);
  pinMode(h2, INPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);
  dht1.begin();
  dht2.begin();
  
  adc.begin(10);
}

void loop() {
  smokesens1 = map(analogRead(smoke1),0,1023,0,255);
  smokesens2 = map(analogRead(smoke2),0,1023,0,255);
  smokesens3 = map(analogRead(smoke3),0,1023,0,255);
  smokesens4 = map(analogRead(smoke4),0,1023,0,255);
  flame1 = digitalRead(f1);
  flame2 = digitalRead(f2);
  flame3 = digitalRead(f3);
  flame4 = digitalRead(f4);
  temp1 = map(adc.readADC(0),0,1023,0,255);
  temp2 = map(adc.readADC(1),0,1023,0,255);
  temp3 = map(adc.readADC(2),0,1023,0,255);
  temp4 = map(adc.readADC(3),0,1023,0,255);
  hum1 = dht1.readHumidity();
  hum2 = dht2.readHumidity();
  water1 = map(adc.readADC(4),0,1023,0,255);
  water2 = map(adc.readADC(5),0,1023,0,255);
}

void receiveData(int bytecount){
  for(int i = 0; i < bytecount; i++){
    receivedData = Wire.read();
  }
}

void sendData(){
  response[0] = (byte)smokesens1;
  response[1] = (byte)smokesens2;
  response[2] = (byte)smokesens3;
  response[3] = (byte)smokesens4;
  response[4] = (byte)temp1;
  response[5] = (byte)temp2;
  response[6] = (byte)temp3;
  response[7] = (byte)temp4;
  response[8] = (byte)flame1;
  response[9] = (byte)flame2;
  response[10] = (byte)flame3;
  response[11] = (byte)flame4;
  response[12] = (byte)hum1;
  response[13] = (byte)hum2;
  response[14] = (byte)water1;
  response[15] = (byte)water2;
  
  Wire.write(response,15);  
}
