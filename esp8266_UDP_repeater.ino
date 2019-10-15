#define IP_FORWARD 1

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#define udp_port 2000
IPAddress SendIP(192,168,4,255); //UDP Broadcast IP data sent to all devicess on same network
WiFiUDP udp;
byte packetBuffer[128];   //Where we get the UDP data
String udp_in = "";
int count = 0;

/* Set these to your desired credentials for ESP8266 AP. */
const char *ssid = "ESPap";
const char *password = "";
IPAddress apIP(192, 168, 4, 1);

//const char* ssidExt = "dlink";
//const char* passwordExt = "";

// Setup credentials for original WiFi, that we plan to repeat
const char* ssidExt = "SmazControl";
const char* passwordExt = "12345678";

void setup()
{
  Serial.begin(115200);
  Serial.println();

   WiFi.mode(WIFI_AP_STA);
   //WiFi.mode(WIFI_AP);

  Serial.println("!!!!!!!!!!!!!!!!!!!++");
  WiFi.begin(ssidExt, passwordExt);

  //Wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssidExt);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("dnsIP address: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("gatewayIP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("subnetMask address: ");
  Serial.println(WiFi.subnetMask());


  Serial.println("");
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  udp.begin(udp_port);
}

void loop()
{
  int cb = udp.parsePacket();
  if (!cb)
  {
    count++;
    count = 0; // never send
    if (count>10) { 
      udp.beginPacket(SendIP, 2000);
      uint8_t a[128];
      memset(a, 0, 128);
      String s = String(random(100));
      Serial.println("Snd: "+s);
      s.getBytes(a,128);
      udp.write(a,s.length()); //Send byte to UDP 
      udp.endPacket();
      count = 0;
    }     
  } else {
    // We've received a UDP packet, send it to serial
    udp.read(packetBuffer, 128); // read the packet into the buffer, we are reading only one byte
    udp_in = String((char *)packetBuffer);
    udp_in.trim();
    memset(packetBuffer, 0, 128);
    Serial.println("Rcv: "+udp_in);
      udp.beginPacket(SendIP, 2000);
      uint8_t a[128];
      memset(a, 0, 128);
      String s = String(udp_in);
      Serial.println("Snd: "+s);
      s.getBytes(a,128);
      udp.write(a,s.length()); //Send byte to UDP 
      udp.endPacket();     
  }  
  delay(1000);
}
