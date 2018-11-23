#include <ESP8266WiFi.h>
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

