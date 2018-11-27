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

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT);
  }
  Serial.begin(9600);

}

void loop() {
  usbMIDI.read();
  buttonsWork();
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
