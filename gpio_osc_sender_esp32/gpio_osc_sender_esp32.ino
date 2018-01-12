// OSC Sender : connects to wifi and sends OSC from GPIO to server

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

#define ENCODER_DO_NOT_USE_INTERRUPTS

//SSID of your network
const char* ssid = "Oddysee"; //SSID of your Wi-Fi router
const char* pass = "123456OK"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,99);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

// Sensors
const int sensorPin = 32;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor
unsigned int oldValue  = -999;


void setup()
{
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.println("Connecting to...");
  Serial.println(ssid);
  
  WiFi.enableSTA(true);
  delay(2000);
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
  sensorValue = analogRead(sensorPin);
  if (sensorValue != oldValue) {
    oldValue = sensorValue;
    //sendOSC("test/", map(sensorValue, 7, 1000, 0, 255));
   }
}

void sendOSC(String msg, unsigned int data) {

  OSCMessage msgOUT(msg.c_str());
  msgOUT.add(data);
  Udp.beginPacket(outIp, outPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
  delay(10);
}

