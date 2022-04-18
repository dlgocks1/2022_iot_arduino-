// set pin numbers
const int buttonPin = 15; // the number of the pushbutton pin
const int ledPin = 22; // the number of the LED pin
// variable for storing the pushbutton status
int buttonState = 0;
char serState = NULL;
const int touchPin = 4;
const int threshold = 30;
int touchValue;
int serailflag = 1;

void setup() {
  Serial.begin(115200);
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

//Task03-1
//void loop() {
//  // read the state of the pushbutton value
//  buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
//  // check if the pushbutton is pressed.
//  // if it is, the buttonState is HIGH
//  if (buttonState == HIGH) {
//    // turn LED on
//    digitalWrite(ledPin, HIGH);
//  } else {
//    // turn LED off
//    digitalWrite(ledPin, LOW);
//  }
//}

//Tast03-2
//void loop() {
//  // read the state of Serial port
//  if (Serial.available() > 0){
//    serState = Serial.read();
//    Serial.println(serState);
//  }
//  // check which keyboard 'h' or 'l' is pressed.
//  if (serState == 'h') {
//    // turn LED on
//    digitalWrite(ledPin, HIGH);
//  } else if (serState == 'l') {
//    // turn LED off
//    digitalWrite(ledPin, LOW);
//  }
//  
//  delay(250);
//}


// <Task 03-3>
// ESP32 Touch Test
// Just test touch pin - Touch0 is T0 which is on GPIO 4.
//void loop()
//{
//  Serial.println(touchRead(touchPin)); // get value of Touch 0 pin = GPIO 4
//  delay(1000);
//}

//Task 03-4, A
//void loop() {
//  touchValue = touchRead(touchPin);
//  buttonState = digitalRead(buttonPin);
//
//  if(touchValue < threshold || buttonState == HIGH) {
//    digitalWrite(ledPin, HIGH);
//  }
//  else {
//    digitalWrite(ledPin, LOW);
//  }
//  
//  delay(200);
//}
//

//Task 03-Step C
void loop() {
  touchValue = touchRead(touchPin);
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    serailflag = serailflag * -1;
    delay(500);
  }
  if (serailflag==1){
    Serial.println(touchValue);
  }
  if(touchValue < threshold) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(200);
}
