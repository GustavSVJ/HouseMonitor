#include <ESP8266WiFi.h>
/*const char* ssid     = "VintherNet";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Briobane";     // The password of the Wi-Fi network*/
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

void WiFiConnect(){
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(".");
  }
}

void MQTTConnect(){
  Serial.println("Connecting to MQTT");

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& payload = jsonBuffer.createObject();
  payload["deviceID"] = deviceID;
  payload["status"] = "disconnected";

  if (!client.connected()) {

      String payloadBuffer;
      payload.printTo(payloadBuffer);
    
    while (!client.connect(deviceID, "HomeMonitor/Devices", 0, false, payloadBuffer.c_str())) {
      delay(1000);
      Serial.print(".");
    }
  }

  payload["status"] = "connected";

  String payloadBuffer;
  payload.printTo(payloadBuffer);
  client.publish("HomeMonitor/Devices", payloadBuffer.c_str(), false);

  Serial.println("MQTT connection established!");
}

char timerFlag = 0;

void ICACHE_RAM_ATTR onTimerISR(){
    timerFlag = 1;
    timer1_write(2400000);//reset 10s timer
}

const byte ledPin = 0;
const byte interruptPin = 4;
volatile byte state = LOW;
unsigned long lastBlinkTime = 0;
unsigned static int compareTime = 100;
int newBlinkFlag = 0;
double power = 0;
double powerAvg = 0;

void setup() {
  pinMode(ledPin, OUTPUT); //debug LED
  pinMode(interruptPin, INPUT_PULLUP);
  Serial.begin(115200);

  //wifi and then MQTT setup
  WiFi.begin(ssid, password);

  WiFiConnect();
  MQTTConnect();

  //Initialize 10s timer
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(2400000); //10s
    
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    WiFiConnect();
  }
  if (!client.connected()){
    MQTTConnect();
  }

  client.loop();


  if (newBlinkFlag == 1){
    newBlinkFlag = 0;

    if (powerAvg == 0){
      powerAvg = power;
    }
    else{
      powerAvg += power;
      powerAvg /= 2;
    }
    Serial.print("PowerAvg: ");
    Serial.println(powerAvg);

    
  }
  
  if (timerFlag == 1 && powerAvg != 0){
    timerFlag = 0;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& payload = jsonBuffer.createObject();
    payload["deviceID"] = deviceID;
    payload["data"] = String(powerAvg);
    powerAvg = 0;
  
    String payloadBuffer;
        
    payload.printTo(payloadBuffer);
    
    client.publish("HomeMonitor/Power", payloadBuffer.c_str(), false);
  }
  else if(timerFlag == 1){
    timerFlag = 0;
  }

  
  


  digitalWrite(ledPin, state);
}

void blink() {
  if (millis() > lastBlinkTime + compareTime) {
    
    double t = (millis() - lastBlinkTime) / 1000.0;
    lastBlinkTime = millis();
    power = 3600 / t;
    Serial.print("Power: ");
    Serial.println(power);
    newBlinkFlag = 1;

  }
}

