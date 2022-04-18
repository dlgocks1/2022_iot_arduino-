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

const int ledChannel = 0;
const int buzPin = 23;
const int duty = 128;
int sVal;
int sdur;
int nFrq[] = {262, 330, 392, 440, 0};
//도 미 솔 도 0
int ndur[] = {500,250,2000};

//Task04-4, A
void setup() {
  Serial.begin(115200);
  ledcAttachPin(buzPin, ledChannel);
}

void loop(){
  if (Serial.available() > 0) {
    sVal = Serial.read();
    delay(50);
    sdur = Serial.read();
    delay(50);
    playNote(sVal-0x30, ndur[sdur-0x30]);
  }
}

void playNote(int note, int dur) {
  ledcSetup(ledChannel, nFrq[note], resolution);
  ledcWrite(ledChannel, duty);
  Serial.println(note);
  delay(dur);
}

// 00010100002031312000313120313120000000503030200030303050303020003030305030302000524030403020100050
