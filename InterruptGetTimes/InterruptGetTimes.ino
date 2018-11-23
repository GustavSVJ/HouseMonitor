const byte ledPin = 0;
const byte interruptPin = 4;
volatile byte state = LOW;
unsigned long lastBlinkTime = 0;
unsigned static int compareTime = 100;

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
    state = !state;
    Serial.println("Blink");
  }
}

