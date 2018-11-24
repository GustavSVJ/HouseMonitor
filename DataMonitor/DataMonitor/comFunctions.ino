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

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& payload = jsonBuffer.createObject();
      payload["deviceID"] = deviceID;
      payload["data"] = indoorTemperatureValue;

      String payloadBuffer;
      
      payload.printTo(payloadBuffer);
    
    while (!client.connect(deviceID, "HomeMonitor/Devices", 0, false, deviceID)) {
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
  if (strcmp(topic, "HomeMonitor/OutdoorTemperature") == 0){
    payload[length] = 0;
    String str = String((char*)payload);
    outdoorTemperatureValue = str.toFloat();
  }
  else if (strcmp(topic, "HomeMonitor/Power") == 0){
    payload[length] = 0;
    String str = String((char*)payload);
    powerValue = str.toFloat();
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

void DHTConnect(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connecting to the");
  display.print("DHT sensor");
  display.display();

  dht.begin();
  
  delay(1000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.println("Failed to connect");
    display.print("to the DHT sensor!");
    display.display();
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connection to the DHT");
  display.print("sensor established!");
  display.display();
  delay(1000);
}

