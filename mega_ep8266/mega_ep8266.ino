                     // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200); // your esp's baud rate might be different
}
 
void loop()
{
  if(Serial2.available()) // check if the esp is sending a message 
  {
    while(Serial2.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial2.read(); // read the next character.
      Serial.write(c);
    }  
  }
  
 
  
  if(Serial.available())
  {
    // the following delay is required because otherwise the arduino will read the first letter of the command but not the rest
    // In other words without the delay if you use AT+RST, for example, the Arduino will read the letter A send it, then read the rest and send it
    // but we want to send everything at the same time.
    delay(1000); 
    
    String command="";
    
    while(Serial.available()) // read the command character by character
    {
        // read one character
      command+=(char)Serial.read();
    }
    Serial2.println(command); // send the read character to the esp8266
  }
}
