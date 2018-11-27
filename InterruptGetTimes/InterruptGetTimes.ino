#include <ESP8266WiFi.h>
const char* ssid     = "VintherNet";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Briobane";     // The password of the Wi-Fi network

//MQTT Libraries
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* deviceID = "ESP8266:Power";

const char* mqttServer = "192.168.0.10";
const int   mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

DynamicJsonBuffer devicejsonBuffer;
JsonObject& devices = devicejsonBuffer.createObject();

WiFiClient wifiClient; //Create wifi client object
PubSubClient client(mqttServer, 1883, wifiClient); //Create a PubSub MQTT Client

const byte ledPin = 0;
const byte interruptPin = 4;
volatile byte state = LOW;
unsigned long lastBlinkTime = 0;
unsigned static int compareTime = 100;
char blinkCounter = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  Serial.begin(115200);

  //wifi and then MQTT setup here
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  if (millis() > lastBlinkTime + compareTime) {
    lastBlinkTime = millis();
    blinkCounter++;
    state = !state;
    Serial.println("Blink");
  }
}

