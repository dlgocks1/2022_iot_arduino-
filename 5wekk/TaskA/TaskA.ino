const int trigPin = 22;
const int echoPin = 23;

// setting PWM properties
//const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 21;
const int duty = 32;

void playNote(int distance, int dur){
  int ren = 600;
  ren = ren-distance;
  if (ren >600){
    ledcSetup(ledChannel, 600, resolution);
    ledcWrite(ledChannel, duty);
  }
  else if (ren <30){
    ledcSetup(ledChannel, 100, resolution);
    ledcWrite(ledChannel, duty);
  }
  else{
    ledcSetup(ledChannel, ren, resolution);
    ledcWrite(ledChannel, duty);
  }
  Serial.print(ren);
  
 // Serial.println(note);
  //delay(dur);
}

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcAttachPin(buzPin, ledChannel);

}


void loop()
{
  long duration, distance;
  //Triggering by 10us pulse
  digitalWrite(trigPin, LOW); // trig low for 2us
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // trig high for 10us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //getting duration for echo pulse
  duration = pulseIn(echoPin, HIGH);
  //sound speed = 340 m/s = 34/1000 cm/us
  //distance = duration * 34/1000 * 1/2
  
  distance = duration * 17 / 1000;
  playNote(distance*6-0x30,100);
  //Serial.print(distance);
  //Serial.print("cm");
  Serial.println();
  delay(100);
}
