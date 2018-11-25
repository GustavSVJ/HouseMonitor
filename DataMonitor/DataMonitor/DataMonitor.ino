#include <ESP8266WiFi.h>        // Include the Wi-Fi library'

const char* ssid     = "VintherNet";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Briobane";     // The password of the Wi-Fi network

//MQTT Libraries
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* deviceID = "ESP8266:Monitor";

const char* mqttServer = "192.168.0.10";
const int   mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

DynamicJsonBuffer devicejsonBuffer;
JsonObject& devices = devicejsonBuffer.createObject();

WiFiClient wifiClient; //Create wifi client object
PubSubClient client(mqttServer, 1883, wifiClient); //Create a PubSub MQTT Client

//DHT Libraries
#include "DHT.h"

#define DHTPIN 12     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN,DHTTYPE);

//Display Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define BITMAP_HOUSE_WIDTH 24
#define BITMAP_HOUSE_HEIGHT 24
// 'IL7N2', 128x24px
const unsigned char bitmapHouse [] PROGMEM = {
  // 'iconfinder_Home_778117, 24x24px
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x3e, 0x38, 0x00, 0x63, 0x38, 0x00, 0xdd, 0xb8, 0x01, 
  0xbe, 0xf8, 0x03, 0x7f, 0x78, 0x06, 0xff, 0xb8, 0x0d, 0xff, 0xd8, 0x1b, 0xff, 0xec, 0x37, 0xc1, 
  0xf6, 0x6f, 0xc1, 0xfb, 0x1f, 0xc1, 0xfc, 0x1f, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0x1f, 0xc1, 0xfc, 
  0x1f, 0xc1, 0xfc, 0x1f, 0xc1, 0xfc, 0x1f, 0xc1, 0xfc, 0x1f, 0xc1, 0xfc, 0x1f, 0xc1, 0xfc, 0x1f, 
  0xc1, 0xfc, 0x1f, 0xc1, 0xfc, 0x00, 0x00, 0x00
};

#define BITMAP_CLOUD_WIDTH 24
#define BITMAP_CLOUD_HEIGHT 24
// 'icon-cloud-blue', 24x24px
const unsigned char bitmapCloud [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
  0xf0, 0x00, 0x03, 0xf8, 0x00, 0x07, 0xff, 0xe0, 0x0f, 0xff, 0xf0, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 
  0xf8, 0x7f, 0xff, 0xf8, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define BITMAP_LIGHTNING_WIDTH 24
#define BITMAP_LIGHTNING_HEIGHT 24
// 'iconfinder_lc_symbolshapes', 24x24px
const unsigned char bitmapLightning [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x80, 0x00, 0x1f, 0x00, 0x00, 0x3e, 0x00, 0x00, 
  0x7c, 0x00, 0x00, 0xf8, 0x00, 0x01, 0xf0, 0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x0f, 0xff, 
  0x80, 0x00, 0x1f, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x08, 0xf8, 0x00, 0x0d, 0xf0, 0x00, 
  0x0f, 0xe0, 0x00, 0x0f, 0xc0, 0x00, 0x0f, 0xc0, 0x00, 0x0f, 0xe0, 0x00, 0x0f, 0xf0, 0x00, 0x0f, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define BITMAP_RAINDROP_WIDTH 24
#define BITMAP_RAINDROP_HEIGHT 24
// '63123', 24x24px
const unsigned char bitmapRaindrop [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x38, 0x00, 0x00, 0x78, 0x00, 0x00, 
  0x7c, 0x00, 0x00, 0xfc, 0x00, 0x01, 0xfe, 0x00, 0x01, 0xff, 0x00, 0x03, 0xff, 0x80, 0x03, 0xff, 
  0x80, 0x07, 0xff, 0xc0, 0x07, 0xff, 0xc0, 0x06, 0xff, 0xe0, 0x04, 0x7f, 0xe0, 0x0c, 0x3f, 0xf0, 
  0x0c, 0x1f, 0xf0, 0x0c, 0x07, 0xf0, 0x04, 0x07, 0xe0, 0x06, 0x03, 0xe0, 0x03, 0x03, 0xc0, 0x01, 
  0x8f, 0x80, 0x00, 0xff, 0x00, 0x00, 0x08, 0x00
};

volatile unsigned long long interruptTimeStamp = 0;
enum menues {indoorTemp, indoorHumidity,outdoorTemp, power};
char activeMenu = indoorTemp;

//Data variables
float indoorTemperatureValue = 0;
float indoorHumidityValue = 0;
boolean outdoorSensorActive = false;
float outdoorTemperatureValue = 0;
boolean powerSensorActive = false;
float powerValue = 0;



void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
 
  WiFi.begin(ssid, password);             // Connect to the network
  
  WiFiConnect();
  MQTTConnect();

  client.setCallback(MQTTCallback);

  MQTTSubscribe("HomeMonitor/Devices/Status");
  MQTTSubscribe("HomeMonitor/OutdoorTemperature");
  MQTTSubscribe("HomeMonitor/Power");
  

  DHTConnect();
  
  pinMode(14, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(14), displayControl, RISING); 

}

void displayControl(){
  if (millis() > interruptTimeStamp + 100){
    interruptTimeStamp = millis();
    if (activeMenu < 3){
      activeMenu++;
    }
    else{
      activeMenu = 0;
    }
  }
}

unsigned long long lastRuntime = 0;
unsigned long long loopTimer = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  if (WiFi.status() != WL_CONNECTED){
    WiFiConnect();
  }
  if (!client.connected()){
    MQTTConnect();
  }

  client.loop();
  
  display.clearDisplay();

  switch(activeMenu){
    case indoorTemp:
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("Indoor Temperature");
      display.drawLine(0,7,128,7,WHITE);
      display.drawBitmap(0, 9, bitmapHouse, BITMAP_HOUSE_WIDTH, BITMAP_HOUSE_HEIGHT, WHITE);
      display.setTextSize(2);
      display.setCursor(35,15);
      display.print(indoorTemperatureValue, 1);
      display.print(" C");
      if (indoorTemperatureValue < 10){
        display.drawCircle(76,17, 2, WHITE);
      }
      else{
        display.drawCircle(88,17, 2, WHITE);
      }
    break;
    case outdoorTemp:
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("Outdoor Temperature");
      display.drawLine(0,7,128,7,WHITE);
      display.drawBitmap(0, 9, bitmapCloud, BITMAP_CLOUD_WIDTH, BITMAP_CLOUD_HEIGHT, WHITE);
      if (outdoorSensorActive){
        display.setTextSize(2);
        display.setCursor(35,15);
        display.print(outdoorTemperatureValue, 1);
        display.print(" C");
        if (outdoorTemperatureValue < 10){
          display.drawCircle(76,17, 2, WHITE);
        }
        else{
          display.drawCircle(88,17, 2, WHITE);
        }    
      }
      else{
        display.setTextSize(1);
        display.setCursor(45,13);
        display.println("Sensor not");
        display.setCursor(57,22);
        display.print("found!");
      }

    break;
    case power:
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("Power Usage");
      display.drawLine(0,7,128,7,WHITE);
      display.drawBitmap(0, 9, bitmapLightning, BITMAP_LIGHTNING_WIDTH, BITMAP_LIGHTNING_HEIGHT, WHITE);
      if (powerSensorActive){
        display.setTextSize(2);
        display.setCursor(35,15);
        display.print(powerValue, 0);
        display.print("W");
      }
      else{
        display.setTextSize(1);
        display.setCursor(45,13);
        display.println("Sensor not");
        display.setCursor(57,22);
        display.print("found!");
      }

    break;
    case indoorHumidity:
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("Indoor Humidity");
      display.drawLine(0,7,128,7,WHITE);
      display.drawBitmap(0, 9, bitmapRaindrop, BITMAP_RAINDROP_WIDTH, BITMAP_RAINDROP_HEIGHT, WHITE);
      display.setTextSize(2);
      display.setCursor(35,15);
      display.print(indoorHumidityValue, 1);
      display.print("%");
    break;
    
  }

  display.display();

  if ((millis() - lastRuntime) > 5000){
    lastRuntime = millis();

    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    
    if(isnan(h) || isnan(t)){
      Serial.println("DHT Sensor is messing up!");
    }
    else{
      if (abs(indoorHumidityValue - h) > 0.1){
        indoorHumidityValue = h;
        
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& payload = jsonBuffer.createObject();
        payload["deviceID"] = deviceID;
        payload["data"] = indoorHumidityValue;
  
        String payloadBuffer;
        
        payload.printTo(payloadBuffer);
    
        client.publish("HomeMonitor/IndoorHumidity", payloadBuffer.c_str(), true);
        
      }
      
      if (abs(indoorTemperatureValue - t) > 0.1){
        indoorTemperatureValue = t;
  
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& payload = jsonBuffer.createObject();
        payload["deviceID"] = deviceID;
        payload["data"] = indoorTemperatureValue;
  
        String payloadBuffer;
        
        payload.printTo(payloadBuffer);
    
        client.publish("HomeMonitor/IndoorTemperature", payloadBuffer.c_str(), true);
        
      } 
    }
  }  
}
