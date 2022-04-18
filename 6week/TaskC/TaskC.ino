// music play station by using pwm
// choi++20200924
// freq -> pitch
// duty -> volume (?)
// setting PWM properties
//const int ledChannel = 0;
//const int resolution = 8;
//const int buzPin = 23;
//const int duty = 128;
//
//// variables
//int vNote = 0, vDur = 0;
//const int dDur = 250; // default duration
//// notes
////enum Notes {C3=0, CS3, D3, DS3, E3, F3};
//int nFrq[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
//int nDur[] = { 2000, 1500, 1000, 750, 500, 375, 250 };
//
//
//void playNote(int note, int dur) {
//  if (note == -1) {
//    ledcSetup(ledChannel, 0, resolution);
//    ledcWrite(ledChannel, 0);
//  }
//  else {
//    ledcSetup(ledChannel, nFrq[note], resolution);
//    ledcWrite(ledChannel, duty);
//  }
//  Serial.println(String(note) + "," + String(dur));
//  delay(nDur[dur]);
//}
//
//void setup() {
//  Serial.begin(115200);
//  // attach the channel to the GPIOs
//  ledcAttachPin(buzPin, ledChannel);
//}
//
//void loop() {
//  if (Serial.available() > 0) {
//    vNote = Serial.read();
//    if (Serial.available() > 0) {
//      vDur = Serial.read();
//      if (vDur <= '6' && vDur >= '0')
//        vDur -= '0';
//      else vDur = dDur;
//      if (vNote <= '9' && vNote >= '0')
//        vNote -= '0';
//      else if (vNote <= 'c' && vNote >= 'a')
//        vNote = vNote - 'a' + 10;
//      else /* if (vNote == ',') */
//        vNote = -1; // rest
//      playNote(vNote, vDur);
//    }
//  }
//}
//04060604044476764404767644767644040404,474744404747474,474744404747474,474744404,0c474c474442404,4

#include <EEPROM.h>
#define EEPROM_SIZE 64

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
int ndur[] = {500, 250, 2000};

int eepromflag = false;
int savenum[64];
int savelen = 0;

void playNote(int note, int dur) {
  ledcSetup(ledChannel, nFrq[note], resolution);
  ledcWrite(ledChannel, duty);
  Serial.println(note);
  delay(dur);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  ledcAttachPin(buzPin, ledChannel);
  //Serial.println(EEPROM.read(2));
  if(EEPROM.read(0) == 170){
      int getlen = EEPROM.read(2);
      int i;
      for (i=0; i<getlen; i++){
        Serial.print(EEPROM.read(3+i));  
      }
      for (i=0; i<getlen; i=i+2){
        playNote(EEPROM.read(3+i),ndur[EEPROM.read(4+i)]);  
      }
      playNote(5,0);

  }
}

void loop() {
  if (Serial.available() > 0) {
    sVal = Serial.read();

    if (sVal == '>') {
      eepromflag = false;
      EEPROM.write(2, savelen);
      EEPROM.write(0, 0xAA); //0xAA : 170 0x55 : 85
      
      //EEPROM.commit();
      int i;
      for (i = 0; i < savelen; i = i + 1) {
        Serial.println(savenum[i]);
        EEPROM.write(3+i, savenum[i]-48); //0xAA : 170 0x55 : 85  
      }
      EEPROM.commit();

      delay(50);
      sVal = Serial.read();
      Serial.println(savelen);
    }
    if (eepromflag == true) {
      savenum[savelen] = sVal;
      savelen += 1;
    }
    //EEPROM.write(0, 0x55); //0xAA : 170 0x55 : 85
    //EEPROM.commit();
    if (sVal == '<') {
      eepromflag = true;
      delay(50);
      sVal = Serial.read();
      savenum[savelen] = sVal;
      savelen += 1;
    }

    delay(50);
    sdur = Serial.read();

    if (eepromflag == true) {
      savenum[savelen] = sdur;
      savelen += 1;
    }

    delay(50);
    playNote(sVal - 0x30, ndur[sdur - 0x30]);
  }
}

//EEPROM.write(0, ledState);
//    EEPROM.commit();


// <00010100>002031312000313120313120
// 00010100002031<3120003131203131>20
// 0001010000203131200031312031312000000050303020<0030303050303020003030305030302000524030>403020100050
// 00010100002031312000313120313120000000503030200030303050303020003030305030302000524030403020100050
