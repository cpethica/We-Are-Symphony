// OSC Sender : connects to wifi and sends OSC from GPIO to server
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads1015;

//SSID of your network
char ssid[] = "boringname"; //SSID of your Wi-Fi router
char pass[] = "Digisteph40br"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,100);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

WiFiUDP Udp1;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp1(192,168,1,100);        // remote IP of your computer
const unsigned int outPort1 = 9998;          // remote port to receive OSC
const unsigned int localPort1 = 8888;        // local port to listen for OSC packets (actually not used for sending)


int sensorPin = A0;    // select the input pin for the potentiometer

// button pins
String osc_address[] = {"/b1", "/b2", "/b3", "/b4", "/b5", "/b6"};

Pushbutton button1(2);
Pushbutton button2(0);
Pushbutton button3(13);
Pushbutton button4(16);
Pushbutton button5(14);
Pushbutton button6(12);

// potentiometer states
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor
unsigned int oldValue  = -999;

void setup()
{
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);

  ads1015.begin();
  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT);

          
  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  Serial.println(WiFi.localIP());

}


void loop() {
 // delay(5);
  sensorValue = analogRead(sensorPin);
  if (sensorValue != oldValue) {
    oldValue = sensorValue;
   // sendOSC("/pot", sensorValue);
    //Serial.println(sensorValue);
   }

  // read the state of the pushbutton values:
  // read the state of the pushbutton values:
  int s[] = {button1.getSingleDebouncedPress(), button2.getSingleDebouncedPress(), button3.getSingleDebouncedPress(), button4.getSingleDebouncedPress(), button5.getSingleDebouncedPress(), button6.getSingleDebouncedPress()};        
  for (int i = 0; i < 6; i++) {
      if (s[i])  {
        Serial.println(osc_address[i]);
        Serial.println(s[i]);
        sendOSC(osc_address[i], s[i]);
      }
           }

  
/// read extra analog pins...  
  int16_t adc0, adc1, adc2, adc3;
  adc0 = ads1015.readADC_SingleEnded(0);
  adc1 = ads1015.readADC_SingleEnded(1);
  adc2 = ads1015.readADC_SingleEnded(2);
  adc3 = ads1015.readADC_SingleEnded(3);

  adc0 = map(adc0, 0, 1023, 0, 12);
  adc1 = map(adc1, 0, 1023, 0, 255);
  adc2 = map(adc2, 0, 1023, 0, 255);
  adc3 = map(adc3, 0, 1023, 0, 255);
  adc0 = constrain(adc0, 0, 255);
  adc1 = constrain(adc1, 0, 255); 
  adc2 = constrain(adc2, 0, 255);
  adc3 = constrain(adc3, 0, 255);
  
  sendOSC("/selector", adc0);
  sendOSC("/slider1", adc1);
  sendOSC("/slider2", adc2);
  sendOSC("/slider3", adc3);

  sendOSC1("/selector", adc0);
  sendOSC1("/slider1", adc1);
  sendOSC1("/slider2", adc2);
  sendOSC1("/slider3", adc3);
  }

void sendOSC(String msg, unsigned int data) {

  OSCMessage msgOUT(msg.c_str());
  msgOUT.add(data);
  Udp.beginPacket(outIp, outPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
}

// second osc send function for alternative OSC port/IP
void sendOSC1(String msg, unsigned int data) {

  OSCMessage msgOUT(msg.c_str());
  msgOUT.add(data);
  Udp1.beginPacket(outIp1, outPort1);
  msgOUT.send(Udp1);
  Udp1.endPacket();
  msgOUT.empty();
}

