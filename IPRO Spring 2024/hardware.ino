/****************************************************************************************************************************
  WebSocketClient_NINA.ino
  For boards using WiFiNINA Shield/Module

  Blynk_WiFiNINA_WM is a library for the Mega, Teensy, SAM DUE, nRF52, STM32 and SAMD boards
  (https://github.com/khoih-prog/Blynk_WiFiNINA_WM) to enable easy configuration/reconfiguration and
  autoconnect/autoreconnect of WiFiNINA/Blynk

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license

  Created on: 24.05.2015
  Author: Markus Sattler
 *****************************************************************************************************************************/

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  // Default pin 10 to SS/CS
  #define USE_THIS_SS_PIN       10
  #define BOARD_TYPE      "SAM DUE"
#elif ( defined(CORE_TEENSY) )
  #error You have to use examples written for Teensy
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    "SAMD MKRWIFI1010"
#endif

#define _WEBSOCKETS_LOGLEVEL_     2
#define WEBSOCKETS_NETWORK_TYPE   NETWORK_WIFININA

#include <string.h>
#include <WiFiNINA_Generic.h>

#include <WebSocketsClient_Generic.h>

#include <Adafruit_NeoPixel.h>

#define LED_PIN_STRIP1  11       // Pin to which the first strip is connected
#define LED_PIN_STRIP2  12       // Pin to which the second strip is connected
#define NUM_LEDS_STRIP1 24       // Number of LEDs in the first strip
#define NUM_LEDS_STRIP2 24       // Number of LEDs in the second strip
#define GAS_SENSOR_PIN   A0      // Pin to which the gas sensor is connected
#define SMOKE_THRESHOLD  1000     // Threshold for smoke detection
#define THERMISTOR_PIN A1        // Pin to which the thermistor is connected
#define TEMPERATURE_THRESHOLD 135 // Threshold for smoke detection  (Change this to 135 after testing)

float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_STRIP1, LED_PIN_STRIP1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_STRIP2, LED_PIN_STRIP2, NEO_GRB + NEO_KHZ800);


WebSocketsClient webSocket;

int status = WL_IDLE_STATUS;

// Deprecated echo.websocket.org to be replaced
#define WS_SERVER           "first-response-server.onrender.com"
#define SSL_PORT            443
#define NODE_ID "00003"
#define MESSAGE_BEGIN "@SOTERIA"
char fire_state[] = "S";
char previous_state[] = "/0";
char x = 'N';
char y = 'S';
char prev_y = 'S';
String message_text = "";
// #define FLOOR_ID "01"


///////please enter your sensitive data in the Secret tab/arduino_secrets.h

char ssid[] = "AndroidAP";        // your network SSID (name)
char pass[] = "123456789";         // your network password (use for WPA, or use as key for WEP), length must be 8+

bool alreadyConnected = false;
bool sent = false;

void webSocketEvent(const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      if (alreadyConnected)
      {
        Serial.println("[WSc] Disconnected!");
        alreadyConnected = false;
      }

      break;

    case WStype_CONNECTED:
    {
      alreadyConnected = true;

      Serial.print("[WSc] Connected to url: ");
      Serial.println((char *) payload);


      message_text = MESSAGE_BEGIN;
      message_text += ",";
      message_text += NODE_ID;
      if(sent == false){
        webSocket.sendTXT(message_text);
        sent = true;
      }

      // send message to server when Connected
      // webSocket.sendTXT("Connected");
    }

    break;

    case WStype_TEXT:
      Serial.print("[WSc] get text: ");
      Serial.println((char *) payload);
      x = ((char *) payload)[0];
      y = ((char *) payload)[1];
      if (x=='N' && y=='S') {
        SetColorIndex(strip2, 0, 23, 0, 0, 255);
        SetColorIndex(strip1, 0, 23, 0, 0, 255);
      }
      else {
        setDirection(x);
        setState(y);
      }
    
      prev_y = ((char *) payload)[1];
      break;

    case WStype_BIN:
      Serial.print("[WSc] get binary length: ");
      Serial.println(length);

      // KH, To check
      // hexdump(payload, length);

      // send data to server
      webSocket.sendBIN(payload, length);

      break;

    case WStype_PING:
      // pong will be send automatically
      Serial.println("[WSc] get ping");

      break;

    case WStype_PONG:
      // answer to a ping we send
      Serial.println("[WSc] get pong");

      break;

    default:
      break;
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Client IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  strip1.begin(); // Initialize the first strip
  strip2.begin(); // Initialize the second strip
  pinMode(GAS_SENSOR_PIN, INPUT); // Set gas sensor pin as input
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // while (!Serial);

  Serial.print("\nStart WebSocketClientSSL_NINA on ");
  Serial.println(BOARD_NAME);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  Serial.println("Used/default SPI pinout:");
  Serial.print("MOSI:");
  Serial.println(MOSI);
  Serial.print("MISO:");
  Serial.println(MISO);
  Serial.print("SCK:");
  Serial.println(SCK);
  Serial.print("SS:");
  Serial.println(SS);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");

    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }

  printWifiStatus();

  // server address, port and URL
  Serial.print("WebSockets Server : ");
  Serial.println(WS_SERVER);

  // server address, port and URL
  webSocket.beginSSL(WS_SERVER, SSL_PORT);

  // event handler
  webSocket.onEvent(webSocketEvent);


  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ ");
  Serial.println(WS_SERVER);
}

void loop()
{
  webSocket.loop();

  int gasValue = analogRead(GAS_SENSOR_PIN);
  //Serial.print("Gas: ");
  //Serial.println(gasValue); // Print gas sensor value to serial monitor
  
  // Read input character for y if available
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (incomingChar != '\n') { // Check if the character is not a newline
      y = incomingChar;
    }
  }

  // send message to server
  message_text = MESSAGE_BEGIN;
  message_text += ",";
  message_text += NODE_ID;
  // message_text += ",";
  // message_text += fire_state;
  if(sent && prev_y != y){
    message_text += ",";
    message_text += y;
    webSocket.sendTXT(message_text);
    prev_y = y;
  }

  // Read temperature sensor value
  int Vo = analogRead(THERMISTOR_PIN);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  //Serial.print("Temperature: "); 
  //Serial.print(T);
  // Serial.println(" F");

  // Check if temperature exceeds 135°F
  if (T > TEMPERATURE_THRESHOLD) {
    y = 'C';
    // setState('C'); // Set state to compromised (red) if temperature exceeds 135°F
  }

  // Check if gas sensor value exceeds threshold
  if (gasValue > SMOKE_THRESHOLD) {
    y = 'C';
    // setState('C'); // Set state to compromised (red) if gas value exceeds threshold
  }
  else if (gasValue < SMOKE_THRESHOLD && x == 'S') {
    y = 'S';
    // setState('S');
  }
  
  strip1.show();
  strip2.show();

  // delay(1000);
}




void setDirection(char x){
  switch (x) {
    case 'U':
      // Set a specific section of strip2 to green
      SetColorIndex(strip2, 0, 5, 0, 0, 0);
      SetColorIndex(strip2, 6, 11, 0, 255, 0);
      SetColorIndex(strip2, 12, 17, 0, 0, 0);
      SetColorIndex(strip2, 18, 23, 0, 255, 0);
      break;

    case 'D':
      // Set a specific section of strip2 to green
      SetColorIndex(strip2, 0, 5, 0, 255, 0);
      SetColorIndex(strip2, 6, 11, 0, 0, 0);
      SetColorIndex(strip2, 12, 17, 0, 255, 0);
      SetColorIndex(strip2, 18, 23, 0, 0, 0);
      break;

    case 'L':
      // Set a specific section of strip2 to green
      SetColorIndex(strip2, 0, 5, 0, 0, 0);
      SetColorIndex(strip2, 6, 17, 0, 255, 0);
      SetColorIndex(strip2, 18, 23, 0, 0, 0);
      break;

    case 'R':
      // Set a specific section of strip2 to green
      SetColorIndex(strip2, 0, 5, 0, 255, 0);
      SetColorIndex(strip2, 6, 17, 0, 0, 0);
      SetColorIndex(strip2, 18, 23, 0, 255, 0);
      break;

    case 'N':
      SetColorIndex(strip2, 0, 23, 0, 0, 0);
      break;

    default:
      // Invalid direction
      break;
  }
}

void setState(char y){
  switch (y) {
    case 'C':
      // Compromised state,
      SetColorIndex(strip1, 0, 23, 255, 0, 0);
      break;
    case 'T':
      // Stuck state,
      SetColorIndex(strip1, 0, 23, 255, 70, 0); // Dark orange
      break;
    case 'S':
      SetColorIndex(strip1, 0, 23, 0, 0, 0);
      break;
    default:
      // Invalid state
      break;
  }
}

void SetColorIndex(Adafruit_NeoPixel &strip, int startIndex, int endIndex, uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = startIndex; i <= endIndex; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
}
