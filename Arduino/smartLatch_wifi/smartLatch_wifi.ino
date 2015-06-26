/***************************************************  
  Dependencies:
  - Adafruit CC3000 Library 
    https://github.com/adafruit/Adafruit_CC3000_Library
  - CC3000 MDNS Library
    https://github.com/adafruit/CC3000_MDNS/
  - Yaler CC3000 Library
  https://yaler.net/arduino-cc3000
  
  This code was adapted from Adafruit CC3000 library example 
  code which has the following license:
  
  Designed specifically to work with the Adafruit WiFi products:
  ----> https://www.adafruit.com/products/1469

 ****************************************************/
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <CC3000_MDNS.h>
#include <Yaler_CC3000_Server.h>

// CC3000 configuration
#define     ADAFRUIT_CC3000_IRQ    3    // MUST be an interrupt pin!
#define     ADAFRUIT_CC3000_VBAT   5    // VBAT and CS can be any two pins
#define     ADAFRUIT_CC3000_CS     10

// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, 
                                         ADAFRUIT_CC3000_IRQ, 
                                         ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);

// Wireless network configuration
#define     WLAN_SSID              "Southern Tide" 
#define     WLAN_PASS              "Fiji1848"
#define     WLAN_SECURITY          WLAN_SEC_WPA2  // Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

//device specifics
#define     DEVICECODE "gsiot-nwn4-nr1x"
#define     DEVICEKEY "i1v9UTgQoKuNF2UBuy2GOWlxJ8E80BlomUUPFM8l"

// Create a server to listen on port 80 (HTTP).
//Adafruit_CC3000_Server server(80);
Yaler_CC3000_Server server("try.yaler.net", 80, DEVICECODE);

// Create a multicast DNS responder.
MDNSResponder mdns;


// Variables to hold current state.
String buffer;
const int connectionPin = 4;
const int motor1 = 6;
const int motor2 = 7;
const int motor3 = 8;
const int motor4 = 9;
int motorPins[] = {motor1, motor2, motor3, motor4};
int revs = 1;
int time = 1000;
int totalSigs = 48 * revs;
int rotation = 0;


void setup() {
  Serial.begin(115200);
  //attachInterrupt(0, irMethod, RISING);
  pinMode(connectionPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
  // Reserve space in the buffer string so there isn't dynamic memory allocation in the loop.
  buffer.reserve(15);
  
  // Connect to the wifi network.
  Serial.println(F("Hello, CC3000!\n")); 
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  Serial.println("Began");  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  Serial.println("Connected");
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }  
  Serial.println("DHCP done");

  // Start multicast DNS responder on 'arduino.local' address.
  if (!mdns.begin("arduino", cc3000)) {
    Serial.println(F("Error setting up MDNS responder!"));
    while(1); 
  }
  Serial.println("mdns began");

  // Start listening for HTTP connections.
  server.begin();
  
  digitalWrite(connectionPin, HIGH);
  Serial.println(F("Listening for connections..."));  
}

void loop() {
  // Update MDNS responder.
  mdns.update();
    
  // Handle any HTTP connections.
  Adafruit_CC3000_ClientRef client = server.available();
  
  //basis for following if statement found at https://learn.adafruit.com/wifi-controlled-led-christmahanukwanzaa-tree/software 
  if (client) {
    buffer = "";
    // Expect "GET /DEVICEKEY/"
    while (client.available() && buffer.length() < 46) {
      buffer = buffer + char(client.read());
    }
    //check for correct device key
    String s = "GET /";
    s += DEVICEKEY;
    s += "/";
    if (buffer != s || !client.available()) {
      client.close();
      Serial.println("error");
      return;
    }

    // parse remaining message for command
    buffer = "";
    char ch = client.read();
    while (client.available() && ch != ' ') {//space signals end of command
      buffer = buffer + ch;
      ch = client.read();
    }
    if (ch != ' ' || !client.available()) { //check if end of message reached w/o finding space signal
      client.close();
      return; 
    }
    // Update appropriate state for the associated command and value.
    if (buffer == "open") {
      rotation = 0;
    } else if (buffer == "close") {
      rotation = 1;
    } else if (buffer == "speed1") { //slow speed
      time = 5000;
    } else if (buffer == "speed2") { //fast speed
      time = 1000;
    } else if (buffer.startsWith("revs")) {
      buffer = buffer.substring(4); //remove 'revs' part to get integer that follows
      revs = buffer.toInt();
      totalSigs = 48 * revs; //48 signals per rotation
      Serial.println(revs);
    } else if (buffer == "go") {
      if (rotation == 0) {
        openLatch();
      } else {
        closeLatch();
      }
    } else if (buffer == "initialize") { //default state
      revs = 1;
      totalSigs = 48 * revs;
      time = 1000;
      rotation = 0;
    }
    client.close();
  }
}

//turns motor clockwise
void openLatch() {
  int lastMotor = 0;
  digitalWrite(motorPins[0], HIGH);
  digitalWrite(motorPins[1], HIGH);
  delay(time / 48); 
  for (int i = 2; i < totalSigs; i++) {
    int currentMotor = i % 4; //so only motor pins 0-3
    digitalWrite(motorPins[lastMotor], LOW);
    digitalWrite(motorPins[currentMotor], HIGH);
    delay(time / 48);
    if (currentMotor == 0) {
      lastMotor = 3;
    } else {
      lastMotor = currentMotor - 1;
    }
  }
  digitalWrite(motorPins[3], LOW);
  digitalWrite(motorPins[2], LOW);
  delay(100);
}

//turns motor counterclockwise
void closeLatch() {
  int lastMotor = 3;
  digitalWrite(motorPins[3], HIGH);
  digitalWrite(motorPins[2], HIGH);
  delay(time / 48);
  for (int i = totalSigs - 3; i >= 0; i--) {
    int currentMotor = i % 4; //so only motor pins 0-3
    digitalWrite(motorPins[lastMotor], LOW);
    digitalWrite(motorPins[currentMotor], HIGH);
    delay(time / 48);
    if (currentMotor == 3) {
      lastMotor = 0;
    } else {
      lastMotor = currentMotor + 1;
    }
  }
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], LOW);
  delay(100);
}
