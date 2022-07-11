#include <Wire.h>
#include <Adafruit_MCP3008.h>
#include <DHT.h>
#define SLAVE_ADDRESS 0x0f
byte receivedData = 0;
byte response[21]; //this is the byte array we will send to the Pi over I2C

int f1 = 0;
int f2 = 1;
int f3 = 2;
int f4 = 3;
int f5 = 4;
int f6 = 5;
int f7 = 6;
int f8 = 7;
int motionsens = 8;
int humsens = 9;

int tempsens = A0;
int watersens = A2;

int flame1;
int flame2;
int flame3;
int flame4;
int flame5;
int flame6;
int flame7;
int flame8;
int smoke1;
int smoke2;
int smoke3;
int smoke4;
int smoke5;
int smoke6;
int smoke7;
int smoke8;

int motion;
int humidity;
int temperature1;
int temperature2;
int water;

Adafruit_MCP3008 adc; 
DHT dht1(humsens,DHT22);
DHT dht2(tempsens,DHT22);

void loop() {
  flame1 = digitalRead(f1);//flame sensors are active LOW
  flame2 = digitalRead(f2);
  flame3 = digitalRead(f3);
  flame4 = digitalRead(f4);
  flame5 = digitalRead(f5);
  flame6 = digitalRead(f6);
  flame7 = digitalRead(f7);
  flame8 = digitalRead(f8);
  smoke1 = (byte)map(adc.readADC(0),0,1023,0,255);//range between 0-255, must find a decent threshold 
  smoke2 = (byte)map(adc.readADC(1),0,1023,0,255);
  smoke3 = (byte)map(adc.readADC(2),0,1023,0,255);
  smoke4 = (byte)map(adc.readADC(3),0,1023,0,255);
  smoke5 = (byte)map(adc.readADC(4),0,1023,0,255);
  smoke6 = (byte)map(adc.readADC(5),0,1023,0,255);
  smoke7 = (byte)map(adc.readADC(6),0,1023,0,255);
  smoke8 = (byte)map(adc.readADC(7),0,1023,0,255);
  motion = (byte)digitalRead(motionsens);//will be 1 if ANY of the motion sensors are triggered
  humidity = (byte)dht1.readHumidity();
  temperature1 = (byte)dht2.readTemperature(true);
  temperature2 = (byte)dht1.readTemperature(true);
  water = (byte)analogRead(watersens);
  delay(2000);
  /*
  Serial.println("Flame Sensors");
  Serial.println(flame1);
  Serial.println(flame2);
  Serial.println(flame3);
  Serial.println(flame4);
  Serial.println(flame5);
  Serial.println(flame6);
  Serial.println(flame7);
  Serial.println(flame8);
  Serial.println("Smoke sensors");
  Serial.println(smoke1);
  Serial.println(smoke2);
  Serial.println(smoke3);
  Serial.println(smoke4);
  Serial.println(smoke5);
  Serial.println(smoke6);
  Serial.println(smoke7);
  Serial.println(smoke8);
  Serial.println("Motion, humidity, temp, water");
  Serial.println(motion);
  Serial.println(humidity);
  Serial.println(temperature1);
  Serial.println(temperature2);
  Serial.println(water);
  delay(3000);
  */
}

void setup() {
  //Serial.begin(9600); for testing
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  pinMode(f1,INPUT);
  pinMode(f2,INPUT);
  pinMode(f3,INPUT);
  pinMode(f4,INPUT);
  pinMode(f5,INPUT);
  pinMode(f6,INPUT);
  pinMode(f7,INPUT);
  pinMode(f8,INPUT);
  pinMode(motionsens,INPUT);
  pinMode(humsens,INPUT);
  pinMode(tempsens,INPUT);
  pinMode(watersens,INPUT);
  dht1.begin();
  dht2.begin();
  adc.begin(10);
  
}

void receiveData(int bytecount){
  for(int i = 0; i < bytecount; i++){
    receivedData = Wire.read();
  }
}

void sendData(){
  response[0] = flame1;
  response[1] = flame2;
  response[2] = flame3;
  response[3] = flame4;
  response[4] = flame5;
  response[5] = flame6;
  response[6] = flame7;
  response[7] = flame8;
  response[8] = smoke1;
  response[9] = smoke2;
  response[10] = smoke3;
  response[11] = smoke4;
  response[12] = smoke5;
  response[13] = smoke6;
  response[14] = smoke7;
  response[15] = smoke8;
  response[16] = motion;
  response[17] = humidity;
  response[18] = temperature1;
  response[19] = temperature2;
  response[20] = water;
  Wire.write(response,20);
}

void siren(int pin){
  for(int i=500;i<750;i++){
  tone(pin,i);
  delay(7);
  }
  noTone(pin);
}

float readingToTemp(int reading){
  float voltage = reading * (3.3/1024.0);
  float temperatureC = (voltage - 0.5) * 100;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  return temperatureF;
}
