#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid     = "VintherNet";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Briobane";     // The password of the Wi-Fi network

//Display Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

volatile int displayCounter = 0;
volatile unsigned long interruptTimeStamp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
 
  WiFi.begin(ssid, password);             // Connect to the network
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connecting to WIFI");
  display.display();

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    display.print(".");
    display.display();
  }
  display.clearDisplay();
  display.setCursor(0,10);
  display.println("WIFI connection");
  display.println("established!");
  display.display();
  delay(5000);
  
  pinMode(14, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(14), displayControl, RISING); 

}

void displayControl(){
  if (millis() > interruptTimeStamp + 100){
    interruptTimeStamp = millis();
    displayCounter++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  
  display.drawLine(0,7,128,7,WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello World!");
  
  display.println(displayCounter);
  display.display();
  
}
