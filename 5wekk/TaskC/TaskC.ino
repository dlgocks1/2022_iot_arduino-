const int ledPin1 = 5; // 5 corresponds to GPIO16
const int ledPin2 = 17; // 17 corresponds to GPIO17
const int ledPin3 = 16; // 6 corresponds to GPIO5
const int motionSensor = 36;

unsigned long ledTime1 = 0;
unsigned long ledTime2 = 0;
unsigned long ledTime3 = 0;
unsigned long nowTime = 0;
int count = 0;

void IRAM_ATTR detectsMovement() {
  Serial.print(count+1);
  Serial.println("번째 감지");
  if (count == 0){
    digitalWrite(ledPin1, HIGH);
    ledTime1 = nowTime;
  }
  else if (count == 1){
    digitalWrite(ledPin2, HIGH);
    ledTime2 = nowTime;
  }
  else if (count == 2){
    digitalWrite(ledPin3, HIGH);
    ledTime3 = nowTime;
  }
  count = (count+1)%3;

}
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

}

void loop() {
  nowTime = millis();
  // put your main code here, to run repeatedly:
  if (nowTime - ledTime1 >= 20000) {
    digitalWrite(ledPin1, LOW);
  }
  if (nowTime - ledTime2 >= 20000) {
    digitalWrite(ledPin2, LOW);
  }
  if (nowTime - ledTime3 >= 20000) {
    digitalWrite(ledPin3, LOW);
  }
//  Serial.println(nowTime);
//  Serial.println(ledTime1);
//  Serial.println(ledTime2);
//  Serial.println(ledTime3);
  delay(1000);
}
