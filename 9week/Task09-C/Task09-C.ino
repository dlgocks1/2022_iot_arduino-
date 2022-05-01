#include <AWS_IOT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_CS 5 // cs for esp32 vspi

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS); // hardware SPI

int delayTime;


const int ledPin = 16;
AWS_IOT testButton;
const char* ssid = "PCBL";
const char* password = "88888888";
char HOST_ADDRESS[]="a27nnf9z5jrak7-ats.iot.ap-northeast-2.amazonaws.com";
char CLIENT_ID[]= "HAECHAN_ESP32";
char sTOPIC_NAME[]= "esp32/led"; // subscribe topic name
char pTOPIC_NAME[]= "esp32/bme280"; // publish topic name
int status = WL_IDLE_STATUS;
int msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
const int buttonPin = 15; // pushbutton pin
unsigned long preMil = 0;
const long intMil = 5000;


void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
  strncpy(rcvdPayload,payLoad,payloadLen);
  rcvdPayload[payloadLen] = 0;
  msgReceived = 1; //플래그 세마포어..
}

void printValues(){
  if((millis()-preMil) > delayTime) {
    preMil = millis();
    
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    
    sprintf(payload," {\"temp\":%d,\"humid\":%d, \"press\":%d}",(int)bme.readTemperature(),
    (int)bme.readHumidity(),(int)bme.readPressure());// 버퍼에 출력할 내용저장 == sprinft
    Serial.println(payload);
    if(testButton.publish(pTOPIC_NAME,payload) == 0) {
      Serial.print("Publish Message:");
      Serial.println(payload);
    }
    else{
      Serial.println("Publish failed");
    }
    
    Serial.println();
  }

  }
  

void setup() {
  
  Serial.begin(115200);
  
  bool status;
  status = bme.begin();
  if(!status){
    Serial.print("BME280 연결실패");
    while(1) delay(10);
  }
  
  //++choi This is here to force the ESP32 to reset the WiFi and initialize correctly.
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode()); //++choi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to wifi");
  if(testButton.connect(HOST_ADDRESS,CLIENT_ID)== 0) {
    Serial.println("Connected to AWS");
    delay(1000);
    if(0==testButton.subscribe(sTOPIC_NAME,mySubCallBackHandler)) {
      Serial.println("Subscribe Successfull");
    }
    else {
      Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
      while(1);
      }
  }
  else {
    Serial.println("AWS connection failed, Check the HOST Address");
    while(1);
  }
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  delayTime = 5000;
  delay(2000);
}

void loop() {
  if(msgReceived == 1) {
    msgReceived = 0;
    Serial.print("Received Message:");
    Serial.println(rcvdPayload);
    // Parse JSON
    JSONVar myObj = JSON.parse(rcvdPayload);
    JSONVar state = myObj["state"];
    String led = (const char*) state["led"];
    Serial.print("LED will be ");
    Serial.println(led);
    if (led == "ON"){
      digitalWrite(ledPin, HIGH);
    }else if (led == "OFF"){
      digitalWrite(ledPin, LOW);
    }
  }
  if((millis()-preMil) > intMil) {
    if (digitalRead(buttonPin)) {
      preMil = millis();
      sprintf(payload,"ESP32-Button:001 Pressed!!");// 버퍼에 출력할 내용저장 == sprinft
      if(testButton.publish(pTOPIC_NAME,payload) == 0) {
        Serial.print("Publish Message:");
        Serial.println(payload);
      }
      else{
        Serial.println("Publish failed");
      }
    }
  }
  printValues();
  
}
