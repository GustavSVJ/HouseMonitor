void WiFiConnect(){
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
  delay(1000);
  
}

void MQTTConnect(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connecting to MQTT");
  display.display();
  delay(1000);
  
  if (!client.connected()) {
    while (!client.connect("ESP8266:Monitor")) {
      delay(1000);
      display.print(".");
      display.display();
    }
  }

  display.clearDisplay();
  display.setCursor(0,10);
  display.println("MQTT connection");
  display.println("established!");
  display.display();
  delay(1000);
}

void MQTTCallback(char* topic, byte* payload, unsigned int length){
  Serial.println("Package received!");
  Serial.println(length);
  
  if (strcmp(topic, "IndoorTemperature") == 0){
    payload[length] = 0;
    String str = String((char*)payload);
    Serial.println(str);
    indoorTemperature = str.toFloat();
  }
  else if (strcmp(topic, "SomeOtherTopic") == 0){
    //Do something
  }
}

void MQTTSubscribe(const char* topic){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Subscribing to MQTT");
  display.print("topics");
  display.display();
  delay(1000);
  
  while (!client.subscribe(topic)) {
    delay(1000);
    display.print(".");
    display.display();
  }
}

