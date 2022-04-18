#include <EEPROM.h>
#define EEPROM_SIZE 1

const int buttonPin = 15; // the number of the pushbutton pin
const int ledPin = 16; // the number of the LED pin

// Variables will change:
int ledState = HIGH; // the current state of the output pin
int buttonState; // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  ledState = EEPROM.read(0);
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}


void loop() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  
  lastButtonState = reading;
  // if the ledState variable is different from the current LED state
  if (digitalRead(ledPin) != ledState) {
    Serial.println("State changed");
    digitalWrite(ledPin, ledState);
    EEPROM.write(0, ledState);
    EEPROM.commit();
    Serial.println(ledState);
    Serial.println(EEPROM.read(0));
    Serial.println("State saved in flash memory");
  }
}
