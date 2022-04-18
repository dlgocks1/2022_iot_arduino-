// the number of the LED pin
const int ledPin = 16; // 16 corresponds to GPIO16
const int ledPin2 = 17; // 17 corresponds to GPIO17
const int ledPin3 = 5; // 5 corresponds to GPIO5
const int buttonPin = 15;

// setting PWM properties
const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int resolution = 8;

//Task04-3
void setup() {
  Serial.begin(115200);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  // attach the channel to the GPIOs
  ledcAttachPin(ledPin, ledChannel1);
  ledcAttachPin(ledPin2, ledChannel2);
  ledcAttachPin(ledPin3, ledChannel3);
  pinMode(buttonPin, INPUT);
}

void loop(){
    // increase the LED brightness
  int j = 0, k = 0, jd = 1, kd = 1;
  for(int i = 0; i <= 1023; i++) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel1, i/4);
    ledcWrite(ledChannel2, j/2);
    if ((j += jd) == 512) jd = -1, j = 511;
    ledcWrite(ledChannel2, k);
    if ((k += kd) >= 256) kd = -1, k = 255;
    else if (k < 0) kd = 1, k = 0;
    checkPause(); delay(5);
  }
  // decrease the LED brightness
  j = 0, k = 0, jd = 1, kd = 1;
  for(int i = 1023; i >= 0; i--) {
    ledcWrite(ledChannel1, i/4);
    ledcWrite(ledChannel2, j/2);
    if ((j += jd) == 512) jd = -1, j = 511;
    ledcWrite(ledChannel3, k);
    if ((k += kd) >= 256) kd = -1, k = 255;
    else if (k < 0) kd = 1, k = 0;
    checkPause(); delay(5);
  }
}
void checkPause() {
  // toggle pause state if button pressed
  if (digitalRead(buttonPin)) {
  Serial.println("in pause..");
  delay(250); // skip glitch
  while (!digitalRead(buttonPin)) ;
  delay(250); // skip glitch
  Serial.println("exit from pause..");
  }
}
