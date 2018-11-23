const byte ledPin = 0;
const byte interruptPin = 4;
volatile byte state = LOW;
unsigned long newTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long SetNewTime = 0;
unsigned long compareTime = 100l;

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
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  newTime = millis();
  if ((newTime - lastBlinkTime) < compareTime) {
    //It is the same blink
  }
  else {
    lastBlinkTime = newTime;
    state = !state;
    delay(200);
    Serial.println("blink ");
    Serial.println(newTime);
  }
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

