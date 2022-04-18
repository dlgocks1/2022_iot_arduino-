// Joystick is connected to GPIO 34/39 (Analog ADC1_CH6/3)
// Cds is connected to GPIO15 (Analog ADC1_CH7)
const int joyPinX = 34;
const int joyPinY = 39;
const int joySW = 36;
const int cdsPin = 35;
// variable for storing the Joystick values
int xValue = 0, yValue = 0, sValue = 0, cValue = 0;
void setup() {
  Serial.begin(115200);
  pinMode(joySW, INPUT);
  delay(1000);
}
void loop() {
// Reading Joystick values
  xValue = analogRead(joyPinX);
  yValue = analogRead(joyPinY);
  cValue = analogRead(cdsPin);
  sValue = digitalRead(joySW);
  String str = "(X,Y) = (" + String(xValue)
  + ',' + String(yValue) + ")\n";
  Serial.print(str);
  Serial.print("Switch = ");
  Serial.println(sValue?"OFF":"ON"); //
  //pull-up circuit
  Serial.print("Cds Value = ");
  Serial.println(cValue);
  delay(500);
}
