// OSC Sender : connects to wifi and sends OSC from GPIO to server
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBundle.h>
#include <HCSR04.h>

// Initialize ultrsasonic sensor
int triggerPin = 16;   //D1 mini pin D0
int echoPin = 14;     // D1 mini pin D5
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);


//SSID of your network
char ssid[] = "Oddysee"; //SSID of your Wi-Fi router
char pass[] = "123456OK"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,100);        // remote IP of your computer
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
  
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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

    
    double distance = distanceSensor.measureDistanceCm();
    int value = map(distance, 0, 399, 0, 254);
    value = constrain(value, 0, 255);
    sendOSC("/distance", value);
    delay(10);
   
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

