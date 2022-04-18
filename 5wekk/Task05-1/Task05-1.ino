const int trigPin = 22;
const int echoPin = 23;
void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  Serial.print(distance);
  Serial.print("cm");
  Serial.println();
  delay(100);
}
