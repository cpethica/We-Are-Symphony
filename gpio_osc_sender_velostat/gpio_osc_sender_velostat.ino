// OSC Sender : connects to wifi and sends OSC from GPIO to server

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Adafruit_NeoPixel.h>

#define PIN 4
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

//SSID of your network
char ssid[] = "boringname"; //SSID of your Wi-Fi router
char pass[] = "Digisteph40br"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,64);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

// SENSORS:
int sensorPin = A0;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor

int colorValue = 0;

void setup()
{
  Serial.begin(115200);
  delay(10);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 // SENSORS:


//   Connect to Wi-Fi network
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
  sensorValue = analogRead(sensorPin);
  int val = map(sensorValue, 7, 1000, 0, 255);
  val = constrain(val, 0, 254);
  sendOSC("vel", val);
  Serial.printf("settings heap size: %u\n", ESP.getFreeHeap());

  colorValue = val; 
  for (int i = 0; i<strip.numPixels(); i++){
    strip.setPixelColor(i, Wheel(colorValue)); //use Wheel function to set color
  }
  strip.show();  
  strip.setBrightness(val); //adjust brightness here              
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 255) {
   return strip.Color(WheelPos , 255 - WheelPos , 0);
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

