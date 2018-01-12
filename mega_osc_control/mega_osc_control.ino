// OSC Sender : connects to wifi and sends OSC from GPIO to server
//#if defined(ESP8266)
#include <ESP8266WiFi.h>
//#else
//#include <WiFi.h>
//#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>



//SSID of your network
char ssid[] = "Oddysee"; //SSID of your Wi-Fi router
char pass[] = "123456OK"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,99);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

int sensorPin = A0;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor
unsigned int oldValue  = -999;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(10);


 // pinMode(sensorPin, INPUT);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  Serial2.println("AT+RST");
  delay(1000);
  Serial2.println("AT+CWJAP=”Oddysee”,”123456OK”");
  delay(1000);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  Serial.println(WiFi.localIP());
}


void loop() {
  delay(100);
  sensorValue = analogRead(sensorPin);
  if (sensorValue != oldValue) {
    oldValue = sensorValue;
    sendOSC(sensorValue);
    //Serial.println(sensorValue);
   }
}

void sendOSC(unsigned int msg) {

  OSCMessage msgOUT("/pot2");
  msgOUT.add(msg);
  Serial.println(msg);
  Udp.beginPacket(outIp, outPort);
  msgOUT.send(Udp);
  Udp.endPacket();
  msgOUT.empty();
  delay(10);
}

