//#define SWAP 0 // sw access point
#include <WiFi.h>
#include "time.h"
#include <EEPROM.h>
#define EEPROM_SIZE 64

const char* ssid = "ESP32_AP_PCBL";
const char* password = "12345678"; // password should be long!!
WiFiServer server(80);
String header;

String output16State = "off";
String output17State = "off";

String AlarmState = "off";

const int output16 = 16;
const int output17 = 17;

const int buzPin = 23;
const int freq = 5000;
const int resolution = 8;
const int ledChannel = 0;
const int duty = 128;
int sVal;
int sdur;
int nFrq[] = {262, 330, 392, 440, 0};
//도 미 솔 도 0
int ndur[] = {500, 250, 2000};

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

unsigned long currentTime2 = 0;
unsigned long alarmtime = 0;

boolean alarmflag =false;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600 * 9; // 3600
const int daylightOffset_sec = 0; // 3600


// alarm
long alarmDelay = -1;
long alarmPlay = 0;
long alarmCntStart = 0;
long alarmCntEnd = 0;

long alarmTime = -1;

// request query index
int idx;
int idxEnd;


void playNote(int note, int dur) {
  ledcSetup(ledChannel, nFrq[note], resolution);
  ledcWrite(ledChannel, duty);
  Serial.println(note);
  delay(dur);
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.println("Year: " + String(timeinfo.tm_year + 1900) + ", Month: " + String(timeinfo.tm_mon + 1));
}


void setup() {
  Serial.begin(115200);
  pinMode(output16, OUTPUT);
  pinMode(output17, OUTPUT);
  digitalWrite(output16, LOW);
  digitalWrite(output17, LOW);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  EEPROM.begin(EEPROM_SIZE);
  ledcAttachPin(buzPin, ledChannel);
  server.begin();

  //connect to WiFi
  //  Serial.printf("Connecting to %s ", ssid);
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }
  //  Serial.println(" CONNECTED");
  //  //init and get the time 동기화
  //  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //  printLocalTime();
  //  //disconnect WiFi as it's no longer needed
  //  WiFi.disconnect(true);
  //  WiFi.mode(WIFI_OFF);

}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }

  WiFiClient client = server.available(); // Listen for incoming clients
  if (client) { // If a new client connects,

    currentTime = millis();
    previousTime = currentTime;


      // alarm play
      if (alarmDelay >= 0) {
        alarmCntEnd = millis();          
        if (currentTime - alarmCntStart >= alarmPlay) {
          // alarm play
          Serial.println("alarm play ");
          if (EEPROM.read(0) == 170) {
          int getlen = EEPROM.read(2);
          int i;
          for (i = 0; i < getlen; i++) {
            Serial.print(EEPROM.read(3 + i));
          }
          for (i = 0; i < getlen; i = i + 2) {
            playNote(EEPROM.read(3 + i), ndur[EEPROM.read(4 + i)]);
          }
          playNote(5, 0);
                
        }

        // alarm initialize
        alarmDelay = -1;
        alarmPlay = 0;
        alarmCntStart = 0;
        alarmCntEnd = 0;
        alarmTime = -1;
        alarmflag = false;
        }
      }

    
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("GPIO 16 on");
              output16State = "on";
              digitalWrite(output16, HIGH);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("GPIO 16 off");
              output16State = "off";
              digitalWrite(output16, LOW);
            } else if (header.indexOf("GET /17/on") >= 0) {
              Serial.println("GPIO 17 on");
              output17State = "on";
              digitalWrite(output17, HIGH);
            } else if (header.indexOf("GET /17/off") >= 0) {
              Serial.println("GPIO 17 off");
              output17State = "off";
              digitalWrite(output17, LOW);
            }
//            if (header.indexOf("GET /alarm/on") >= 0) {
//              AlarmState = "on";
//              currentTime2 = timeinfo.tm_sec;
//              if (alarmflag == false){
//                   alarmtime = currentTime2 + 5;
//                   alarmflag = true;
//              }
//              //delay(5000);
////             
//              if (alarmtime < currentTime2) {
//                if (EEPROM.read(0) == 170) {
//                  int getlen = EEPROM.read(2);
//                  int i;
//                  for (i = 0; i < getlen; i++) {
//                    Serial.print(EEPROM.read(3 + i));
//                  }
//                  for (i = 0; i < getlen; i = i + 2) {
//                    playNote(EEPROM.read(3 + i), ndur[EEPROM.read(4 + i)]);
//                  }
//                  playNote(5, 0);
//                  alarmflag = false;
//                }
//              }
//            } else if (header.indexOf("GET /alarm/off") >= 0) {
//              AlarmState = "off";
//            }

            // alarm set data ( client -> server)
            if (header.indexOf("?alarm=") >=0 && alarmflag == false) {
              alarmCntStart = millis();
              Serial.println("alarm set ");
              idx = header.indexOf("=");
              idx += 1;
              idxEnd = header.indexOf(" HTTP/1.1");
              alarmDelay = header.substring(idx, idxEnd).toInt();
              alarmPlay = alarmDelay * 1000;
              
              alarmTime = timeinfo.tm_sec + alarmDelay;
              alarmflag = true;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50;border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            // Display current state, and ON/OFF buttons for GPIO 16
            client.println("<p>GPIO 16 - State " + output16State + "</p>");
            // If the output16State is off, it displays the ON button
            if (output16State == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            // Display current state, and ON/OFF buttons for GPIO 17
            client.println("<p>GPIO 17 - State " + output17State + "</p>");
            // If the output17State is off, it displays the ON button
            if (output17State == "off") {
              client.println("<p><a href=\"/17/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
//            if (AlarmState == "off") {
//              client.println("<p><a href=\"/alarm/on\"><button class=\"button\">Set Alarm On 5Seconds Later</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/alarm/off\"><button class=\"button\">Back</button></a></p>");
//            }

            // RTP
            client.println("<script>var totalTime=" + String(timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec) +
                           "; setInterval(function(){totalTime++; document.getElementById('timer').innerHTML='NowTime: '+Math.floor(totalTime/3600) + ':' + Math.floor(totalTime%3600/60) + ':' + totalTime%3600%60;}, 1000);</script>");
            client.println(&timeinfo, "<h2 id='timer'>NowTime: %H:%M:%S</h2>");
            client.println("Year: " + String(timeinfo.tm_year + 1900) + ", Month: " + String(timeinfo.tm_mon + 1));

            client.println("</br> Alarm seted Seconds: " + String(alarmTime));

            // alarm
            client.println("<form>alarm set (after this sec): <input type='text' name='alarm'/><br>");
            client.println("<input type='submit' value='SET'></form>");



            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } //** if (currentLine.length() == 0) {
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } //** if (c == '\n') {
        else if (c != '\r') { // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      } //* if (client.available()) {
    } //** while
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  } //** if (client) {
} //** loop() {
