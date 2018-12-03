int buttonPin[4] = {1, 2, 3, 4};
int potPin[4] = {A8, A7, A6, A5};

int potVal[4] = {0, 0, 0, 0};
int lastPotVal[4] = {0, 0, 0, 0};
int ccVal[4] = {0, 0, 0 , 0};
int ccNumber[4] = {1, 2 , 3, 4};

unsigned long lastCCTime[4] = {0, 0, 0, 0};

boolean lastButtonStateArray[4] = {LOW, LOW, LOW, LOW};
boolean buttonStateArray[4] = {LOW, LOW, LOW, LOW};
unsigned long lastButtonStateChange;


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=60,56
AudioEffectBitcrusher    bitcrusher1;    //xy=233,73
AudioFilterStateVariable filter1;        //xy=406,119
AudioFilterStateVariable filter2;        //xy=563,137
AudioEffectFade          fade1;          //xy=621,240
AudioOutputAnalog        dac1;           //xy=755,316
AudioConnection          patchCord1(adc1, bitcrusher1);
AudioConnection          patchCord2(bitcrusher1, 0, filter1, 0);
AudioConnection          patchCord3(filter1, 0, filter2, 0);
AudioConnection          patchCord4(filter2, 2, fade1, 0);
AudioConnection          patchCord5(fade1, dac1);
// GUItool: end automatically generated code




void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT);
  }
  Serial.begin(9600);
  AudioMemory(12);
}

void loop() {
  usbMIDI.read();
  //buttonsWork();
  button1Audio();
  button2Audio();
  button3Audio();
  button4Audio();

}

void buttonsWork() {
  for (int i = 0; i < 4; i++) {

    if (digitalRead(buttonPin[i]) == HIGH) {
      effectOn(i);
    }


    if (millis() > lastButtonStateChange + 50) {
      lastButtonStateArray[i] = buttonStateArray[i];
      buttonStateArray[i] = digitalRead(buttonPin[i]);

      if (buttonStateArray[i] == LOW  && lastButtonStateArray[i] == HIGH) {
        lastButtonStateChange = millis();
        //turn something off
        effectOff(i);
        Serial.println("turn off");

      }
    }

  }
}

void  effectOn(int ccNum) {

  Serial.println(ccNum);

  lastPotVal[ccNum] = potVal[ccNum];
  potVal[ccNum] = analogRead(potPin[ccNum]);
  if (potVal[ccNum] != lastPotVal[ccNum] && millis() > lastCCTime[ccNum] || 0) {
    lastCCTime[ccNum] = millis();
    ccVal[ccNum] = map(potVal[ccNum], 0, 1023, 0, 127);
    usbMIDI.sendControlChange(ccNumber[ccNum], ccVal[ccNum], 1);
    Serial.print(ccNum);
    Serial.print(", ");
    Serial.println(ccVal[ccNum]);
  }
}



void  effectOff(int ccNum) {

  usbMIDI.sendControlChange(ccNumber[ccNum], 0, 1);
      Serial.println(ccVal[ccNum]);
}









void button1Audio() {

  if (digitalRead(buttonPin[0]) == HIGH) {
    bitcrusher1.sampleRate(3000);
    bitcrusher1.bits(2);
  }
  else {
    bitcrusher1.sampleRate(44100);
    bitcrusher1.bits(16);
  }
}


void button2Audio() {

  if (digitalRead(buttonPin[1]) == HIGH) {
    filter1.frequency(1000);
    filter1.resonance(1);
    filter1.octaveControl(2.5);
  }
  else {
    filter1.frequency(15000);
  }
}

void button3Audio() {

  if (digitalRead(buttonPin[2]) == HIGH) {
    filter2.frequency(3000);
    filter2.resonance(1);
    filter2.octaveControl(2.5);
  }
  else {
    filter2.frequency(100);
  }
}


void button4Audio() {

  if (digitalRead(buttonPin[3]) == HIGH) {
    fade1.fadeOut(500);
  }
  else {
    fade1.fadeIn(100);
  }
}
