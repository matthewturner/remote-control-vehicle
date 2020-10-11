int ledRedPin = 13;
int ledGreenPin = 12;
int motorLeftForwardPin = 9;
int motorLeftReversePin = 10;
int motorRightForwardPin = 7;
int motorRightReversePin = 8;
int inPin = 2;
int val = 0;

void setup() {
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(motorLeftForwardPin, OUTPUT);
  pinMode(motorLeftReversePin, OUTPUT);
  pinMode(motorRightForwardPin, OUTPUT);
  pinMode(motorRightReversePin, OUTPUT);
  pinMode(inPin, INPUT);
}

void loop() {
  val = digitalRead(inPin);
  if (val == HIGH) {
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(motorLeftForwardPin, HIGH);
    digitalWrite(motorLeftReversePin, LOW);
    digitalWrite(motorRightForwardPin, HIGH);
    digitalWrite(motorRightReversePin, LOW);
  } else {
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledRedPin, LOW);
    digitalWrite(motorLeftForwardPin, LOW);
    digitalWrite(motorLeftReversePin, HIGH);
    digitalWrite(motorRightForwardPin, LOW);
    digitalWrite(motorRightReversePin, HIGH);
  }
}
