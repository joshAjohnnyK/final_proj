int buttonPin = 1;
int potPin = A8;

int potVal = 0;
int lastPotVal = 0;
int ccVal = 0;
int ccNumber = 1;

boolean lastButtonState;
boolean buttonState;
unsigned long lastButtonStateChange;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);

}

void loop(){
  buttonOneWork();
  effectOneOn();
}

void buttonOneWork() {
  if (millis() > lastButtonStateChange + 50){
    lastButtonState = buttonState;
    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW && lastButtonState == HIGH) {
      lastButtonStateChange = millis();      
      Serial.println("do nothing");
      //turn something off
    }else if (buttonState == HIGH && lastButtonState == LOW) {
      lastButtonStateChange = millis();
      //turn something on
      effectOneOn();
      Serial.println("turn on");
      
    }
  }

}

void  effectOneOn(){
  lastPotVal = potVal;
  potVal = analogRead(potPin);
  if(potVal != lastPotVal) {
    ccVal = map(potVal, 0, 1023, 0, 127);
    usbMIDI.sendControlChange(ccNumber, ccVal, 2);
    Serial.println(ccVal);
  }
}
