// OSC Sender : connects to wifi and sends OSC from GPIO to server

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
//#include <OSCBundle.h>


//SSID of your network
const char* ssid     = "boringname";
const char* password = "Digisteph40br";

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,64);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

int sensorPin = A0;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor
unsigned int oldValue  = -999;

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
  delay(20);
  sensorValue = analogRead(sensorPin);
  sendOSC("/data", sensorValue);
  //Serial.println(sensorValue);
}

void sendOSC(String msg, unsigned int data) {

  OSCMessage msgOUT(msg.c_str());
  msgOUT.add(data);
  Udp.beginPacket(outIp, outPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
  delay(1);
}

