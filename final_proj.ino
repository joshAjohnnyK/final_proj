int buttonPin[4] = {1, 2, 3, 4};
int potPin[4] = {A8, A7, A6, A4};
//

int potVal[4] = {0, 0, 0, 0};
int lastPotVal[4] = {0, 0, 0, 0};
int ccVal[4] = {0, 0, 0 , 0};
int ccNumber[4] = {1, 2 , 3, 4};
int channelNumber[4] = {1, 2, 3, 4};

unsigned long lastCCTime[4] = {0, 0, 0, 0};

boolean lastButtonStateArray[4] = {HIGH, HIGH, HIGH, HIGH};
boolean buttonStateArray[4] = {LOW, LOW, LOW, LOW};
unsigned long lastButtonStateChange;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT);
  pinMode(potPin[i], INPUT);
  }
  Serial.begin(9600);

}

void loop() {
  usbMIDI.read();
  buttonWork();
}

void buttonWork() {
  for (int i = 0; i < 4; i++) {
    if (millis() > lastButtonStateChange + 50) {
      lastButtonStateArray[i] = buttonStateArray[i];
      buttonStateArray[i] = digitalRead(buttonPin[i]);

      if (buttonStateArray[i] == HIGH && lastButtonStateArray[i] == LOW) {
        lastButtonStateChange = millis();
        Serial.println("turn on");
        effectOn();
        //turn something on
      } else if (buttonStateArray[i] == LOW  && lastButtonStateArray[i] == HIGH) {
        lastButtonStateChange = millis();
        //turn something off
        effectOff();
        Serial.println("turn off");

      }
    }
  }

}

void  effectOn() {

  for (int i = 0; i < 4; i++) {

    lastPotVal[i] = potVal[i];
    potVal[i] = analogRead(potPin[i]);
    if (potVal[i] != lastPotVal[i] && millis() > lastCCTime[i] || 0) {
      lastCCTime[i] = millis();
      ccVal[i] = map(potVal[i], 0, 1023, 0, 127);
      usbMIDI.sendControlChange(ccNumber[i], ccVal[i], channelNumber[i]);
      Serial.println(ccVal[i]);
    }
  }
}

void  effectOff() {

  for (int i = 0; i < 4; i++) {

    lastPotVal[i] = potVal[i];
    potVal[i] = analogRead(potPin[i]);
    if (potVal[i] != lastPotVal[i] && millis() > lastCCTime[i] || 0) {
      lastCCTime[i] = millis();
      ccVal[i] = map(potVal[i], 0, 1023, 0, 127);
      usbMIDI.sendControlChange(ccNumber[i], 0, channelNumber[i]);
      Serial.println(ccVal[i]);
    }
  }
}
