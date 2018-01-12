//#include <SoftwareSerial.h>
   //use mega Serial 2 for serial monitor; Serial 1 on pins 19 (RX) and 18 (TX);// Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX).
   #define SSID "PLUSNET-CTX3"
   #define PASS "58e685a7b5"
   #define DST_IP "220.181.111.85" //baidu.com
   //SoftwareSerial dbgSerial(10, 11); // RX, TX
   void setup()
   {
     // Open serial communications and wait for port to open:
     //serial 2 is to esp8266 
     Serial2.begin(115200);//9600 (mine), 57600, 115200
     Serial2.setTimeout(5000);

     //serial 0 is to usb
     Serial.begin(115200);


     while(!Serial); 
     while(!Serial2);

     //dbgSerial.begin(9600); //can't be faster than 19200 for softserial
     //dbgSerial.println("ESP8266 Demo");
     Serial.println("ESP8266 Demo on Mega2560");

    while(Serial2.available()>0)
    Serial2.read();

     delay(1000);
       //test if the module is ready
     Serial2.println("AT+RST");
     delay(1000);
     //delay(1000);
     Serial.println("Resetting module");
     Serial2.flush();
     //Serial.println(Serial2.read());
     //if(Serial2.find("ready"))
     if(Serial2.find("Ready")||Serial2.find("ready"))
     {
       //dbgSerial.println("Module is ready");
       Serial.println("Module is ready");
     }
     else
     {
       //dbgSerial.println("Module have no response.");
       Serial.println("Module have no response.");
       //while(1);
     }
     delay(1000);
     //connect to the wifi
     boolean connected=false;
     for(int i=0;i<5;i++)
     {
       if(connectWiFi())
       {
         connected = true;
         break;
       }
     }
     if (!connected){
      //while(1);
      Serial.println("Not Connected.");
     }
     delay(1000);
     //print the ip addr

   Serial2.println("AT+CIFSR");
     Serial.println("ip address:");
     while (Serial2.available())
     Serial.write(Serial2.read());


     //set the single connection mode
     Serial2.println("AT+CIPMUX=0");
   }
   void loop()
   {
    //connectWiFi();
     String cmd = "AT+CIPSTART=\"TCP\",\"";
     cmd += DST_IP;
     cmd += "\",80";
     Serial2.println(cmd);
     Serial.println(cmd);
     Serial.println(cmd);
     if(Serial2.find("Error")) return;
     cmd = "GET / HTTP/1.0\r\n\r\n";
     Serial2.print("AT+CIPSEND=");
     Serial2.println(cmd.length());
     if(Serial2.find(">"))
     {
       //dbgSerial.print(">");
       Serial.print(">");
       }else
       {
         Serial2.println("AT+CIPCLOSE");
         //dbgSerial.println("connect timeout");
         Serial.println("connect timeout");
         delay(1000);
         return;
       }
       Serial2.print(cmd);
       delay(2000);
       //Serial.find("+IPD");
       while (Serial2.available())
       {
         char c = Serial2.read();
         //dbgSerial.write(c);
         Serial.write(c);
         //if(c=='\r') dbgSerial.print('\n');
         if(c=='\r') Serial.print('\n');
       }
       //dbgSerial.println("====");
       Serial.println("====");
       delay(1000);
     }
     boolean connectWiFi()
     {
       Serial2.println("AT+CWMODE=1");
       String cmd="AT+CWJAP=\"";
       cmd+=SSID;
       cmd+="\",\"";
       cmd+=PASS;
       cmd+="\"";
       //dbgSerial.println(cmd);
       Serial2.println(cmd);
       Serial.println(cmd);
       delay(2000);
       if(Serial2.find("OK"))
       {
         //dbgSerial.println("OK, Connected to WiFi.");
         Serial.println("OK, Connected to WiFi.");
         return true;
         }else
         {
           //dbgSerial.println("Can not connect to the WiFi.");
           Serial.println("Can not connect to the WiFi.");
           return false;
         }
       }
