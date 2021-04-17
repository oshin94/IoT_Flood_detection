/* This is the code for the NodeMCU which acts as a server */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ThingSpeak.h>

const char *ssid = "BSNL_AP";
const char *password = "paul@choudhury";
const char *api_key = "D51504F6BXBU3JFZ";
unsigned long channelNumber = 1183121;
WiFiClient  client;

ESP8266WebServer server(80);

void handleSentVar() {
  if (server.hasArg("sensor1_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor1_reading").toInt();
    Serial.println("Sens1: "+(String)readingInt); //Send to uno and print on serial monitor
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/html", "Data received"); //Respond back to end node
    ThingSpeak.writeField(channelNumber, 1, readingInt, api_key); //Send to ThingSpeak
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (server.hasArg("sensor2_reading")) { // this is the variable sent from the client
    int readingInt = server.arg("sensor2_reading").toInt();
    Serial.println("Sens2: "+(String)readingInt); //send to uno and print on serial monitor
    digitalWrite(LED_BUILTIN, LOW);
    server.send(200, "text/html", "Data received"); //Respond back to end node
    ThingSpeak.writeField(channelNumber, 2, readingInt, api_key); //Send to ThingSpeak
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup() {
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  //configuration for the server
  IPAddress staticIP(192, 168, 1, 100);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(8, 8, 8, 8);
 
  WiFi.mode(WIFI_STA);
  WiFi.config(staticIP, gateway, subnet, dns);
  ThingSpeak.begin(client);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
}
