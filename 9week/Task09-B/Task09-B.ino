#include <AWS_IOT.h>
#include <WiFi.h>
AWS_IOT testButton;
const char* ssid = "PCBL";
const char* password = "88888888";
char HOST_ADDRESS[]="a2xykh7oin9u65-ats.iot.us-west-2.amazonaws.com";
char CLIENT_ID[]= "HAECHAN_ESP32";
char sTOPIC_NAME[]= "esp32/buttAck"; // subscribe topic name
char pTOPIC_NAME[]= "esp32/button"; // publish topic name
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

void setup() {
  Serial.begin(115200);
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
  delay(2000);
}

void loop() {
  if(msgReceived == 1) {
    msgReceived = 0;
    Serial.print("Received Message:");
    Serial.println(rcvdPayload);
  }
  if((millis()-preMil) > intMil) {
    // read the state of the pushbutton value
    if (digitalRead(buttonPin)) {
      preMil = millis();
      sprintf(payload,"ESP32-Button:001 Pressed!!");
      if(testButton.publish(pTOPIC_NAME,payload) == 0) {
        Serial.print("Publish Message:");
        Serial.println(payload);
      }
      else{
        Serial.println("Publish failed");
      }
    }
  }
}
