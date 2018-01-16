// OSC Sender : connects to wifi and sends OSC from GPIO to server

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Encoder.h>

#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;


//SSID of your network
char ssid[] = "Oddysee"; //SSID of your Wi-Fi router
char pass[] = "123456OK"; //Password of your Wi-Fi router

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,100);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

// SENSORS:
int sensorPin = A0;    // select the input pin for the potentiometer
unsigned int sensorValue = 0;  // variable to store the value coming from the sensor
unsigned int DegValue = 0;  // variable to store the value coming from the sensor
Encoder myEnc(14, 12);
unsigned int oldenc  = -999;
unsigned int oldvalue = -999;

void setup()
{
  Serial.begin(115200);
  delay(10);

   Serial.begin(9600);

  // Initialize Initialize HMC5883L
  Serial.println("Initialize HMC5883L");
  while (!compass.begin())
  {
    Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(102, 194);

 // SENSORS:


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

 Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (3.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180/M_PI;

  // Output
 // Serial.print(" Heading = ");
 // Serial.print(heading);
 // Serial.print(" Degress = ");
 // Serial.print(headingDegrees);
 // Serial.println();

  //delay(100);


  sensorValue = analogRead(sensorPin);
  //Serial.println(map(sensorValue, 7, 1000, 0, 255));
  if (sensorValue != oldvalue) {
    oldvalue = sensorValue;
    sendOSC("pot2", map(sensorValue, 7, 1000, 0, 255));
  }

   DegValue = headingDegrees;
  //Serial.println(map(DegValue, 7, 1000, 0, 255));
  if (DegValue != oldvalue) {
    oldvalue = DegValue;
    sendOSC("Deg", map(DegValue, 0, 360, 0, 255));
  }


  unsigned int newenc = myEnc.read();
  if (newenc != oldenc) {
    oldenc = newenc;
    sendOSC("enc", newenc);
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
