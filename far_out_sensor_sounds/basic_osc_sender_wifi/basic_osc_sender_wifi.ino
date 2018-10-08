// OSC Sender : connects to wifi and sends OSC from GPIO to server

#include <WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>  
#include <WiFiUdp.h>
#include <OSCMessage.h>

//SSID of your network
const char* ssid     = "boringname";
const char* password = "Digisteph40br";

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,64);        // remote IP of your computer
const unsigned int outPort = 57120;          // remote port to receive OSC

int sensorPin = 32;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor

void setup()
{
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);

  pinMode(sensorPin, INPUT);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  Serial.println(WiFi.localIP());

}

void loop() {
  delay(10);
  sendOSC("/data", analogRead(sensorPin));
  //Serial.println(sensorValue);
}

void sendOSC(String msg, unsigned int data) {

  OSCMessage msgOUT(msg.c_str());
  msgOUT.add(data);
  Udp.beginPacket(outIp, outPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
}


