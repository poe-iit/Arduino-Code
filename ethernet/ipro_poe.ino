#include <Adafruit_MCP3008.h>
#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>

// TO DO:
// CLEAN UP CODE
// USE DHCP
// ADD AUTO-CONNECT FEATURE

enum opts : byte {INITIALIZATION, POST};
enum sensorType : byte {FIRE, SMOKE, MOTION, HUMIDITY, TEMPERATURE, WATER};

byte response[21];
sensorType responseType[21];

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

String ca = ", ";

Adafruit_MCP3008 adc; 
DHT dht1(humsens,DHT22);
DHT dht2(tempsens,DHT22);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
// Change to DHCP later
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 89, 21, 188);

// Enter the IP address of the server you're connecting to:
//IPAddress server(169, 254, 114, 35);
IPAddress server(10, 89, 21, 103);
int port = 1234;

EthernetClient client;

void setup() {
  Serial.begin(9600); //for debugging
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

  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // start the Ethernet connection:
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  while (!Serial) { 
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    delay(500);
  }

  // give the Ethernet shield a second to initialize:
  Serial.println("Initalizing...");
  Serial.println();
  delay(3000);
  //////////////////////////////////////////////////////////


}

void loop() {
  check_connection();
  check_incoming_data();
  
  check_sensors();
  post_data();
  //byte buf[] = { 0xDE, 0xAD, 0xBE, 0xEF };
  //upload_data(buf, 4);

  delay(2000);
  Serial.println("Flame Sensors");
  Serial.println(flame1 + ca + flame2 + ca + flame3 + ca + flame4 + ca + flame5 + ca + flame6 + ca + flame7 + ca + flame8);
  Serial.println("Smoke sensors");
  Serial.println(smoke1 + ca + smoke2 + ca + smoke3 + ca + smoke4 + ca + smoke5 + ca + smoke6 + ca + smoke7 + ca + smoke8);
  Serial.println("Motion, humidity, temp, water");
  Serial.println(motion + ca + humidity+ ca + temperature1 + ca + temperature2 + ca + water);
}

// Handles linked unplugged after some time
void check_connection() {
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("No Connection");
    client.stop();
    // do nothing:
    while (true) {
      delay(1000);
      // if you get a connection, report back via serial:
      if (client.connect(server, port)) {
        Serial.println("Connected");
        break;
      } else {
        // if you didn't get a connection to the server:
        Serial.println("Connection Failed");
        break;
      }
    }
  }
}

void check_incoming_data() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}

// Format as [ byte operation, byte ID, byte Sensor Type, byte Sensor Reading ]
//
// 4 Bytes (i.e. int) due to better space utilization in packet
// and better support for all network devices
//
// support concatenating byte array into a single packet with 512 byte limit as to
// not overload the buffer & 128 independent commands! more than enough per arduino
// commands are concatenated as follows send([int][int][byte*4][int])
void upload_data(byte buf[], int len) {
  if (client.connected()) {
    client.flush();

    //byte buf[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    //int len = 4;
    //sizeof(buf)
    
    client.write(buf, len);
  } 
}

void check_sensors() {
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
  
  reformat_data();
}

/*byte* construct_packet(byte operation, byte sensorId, byte sensorType, byte sensorReading) {
  byte buf[] = { operation, sensorId, sensorType, sensorReading };
  return buf;
}*/

void post_data() {
  for (int i = 0; i < sizeof response/ sizeof response[0]; i++) {
    byte buf[] = { POST, (byte)i, responseType[i], response[i] };
    upload_data(buf, 4);
  }
}

void reformat_data(){
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
  responseType[0] = FIRE;
  responseType[1] = FIRE;
  responseType[2] = FIRE;
  responseType[3] = FIRE;
  responseType[4] = FIRE;
  responseType[5] = FIRE;
  responseType[6] = FIRE;
  responseType[7] = FIRE;
  responseType[8] = SMOKE;
  responseType[9] = SMOKE;
  responseType[10] = SMOKE;
  responseType[11] = SMOKE;
  responseType[12] = SMOKE;
  responseType[13] = SMOKE;
  responseType[14] = SMOKE;
  responseType[15] = SMOKE;
  responseType[16] = MOTION;
  responseType[17] = HUMIDITY;
  responseType[18] = TEMPERATURE;
  responseType[19] = TEMPERATURE;
  responseType[20] = WATER;
}

///////////////////////////////////////////////////

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
  return temperatureC;
}
