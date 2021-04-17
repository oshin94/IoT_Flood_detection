/* This is the code for the NodeMCU in location B which acts as a transmitter */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char *ssid = "BSNL_AP";
const char *password = "paul@choudhury";

const int gpio4 = 4;  //pin for getting sensor input
int sensorValue = 0;  //store sensor value
String sec;

// Define NTP Client to get time
const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(10);

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  timeClient.begin();
}

void loop() {
  // read the sensor value:
  sensorValue = digitalRead(gpio4);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.1.100";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  //create a URI for request. /data/?sensor2_reading=0
  String url = "/data/";
  url += "?sensor2_reading=";
  url += (String)sensorValue;

  sec = timeClient.getSeconds();
  if (sec.toInt() % 10 == 0 ){
    digitalWrite(LED_BUILTIN, LOW);
    // This will send a request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    digitalWrite(LED_BUILTIN, HIGH);
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 2000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  }

  delay(1000);
}
