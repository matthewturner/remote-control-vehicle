#include <L293.h>
#include <IRremote.h>

int NUMBER_OF_LEDS = 3;
int ledRedPin = 12;
int ledGreenPin = 10;
int ledBluePin = 11;
int leds[] = { ledGreenPin, ledRedPin, ledBluePin };

int irSensorPin = 4;
int ledOnboardPin = 13;
int motorLeftEnablePin = 9;
int motorLeftForwardPin = 7;
int motorLeftReversePin = 8;
int motorRightEnablePin = 3;
int motorRightForwardPin = 6;
int motorRightReversePin = 5;

L293 motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin);
L293 motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin);

IRrecv irrecv(irSensorPin);
decode_results results;

bool isMoving = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledOnboardPin, OUTPUT);

  irrecv.enableIRIn();

  turnOffAllLeds();
  cycleThroughLeds();
}

void loop() {
  if (irrecv.decode(&results)) {
    toggleOn(ledBluePin);
    Serial.println(results.value, HEX);
    irrecv.resume();
    delay(100);
    toggleOn(ledGreenPin);
  }

  switch(results.value) {
    case 0xFF02FD:
      if (isMoving) {
        stop();
      } else {
        moveForward(5);
      }
      break;
  }
}

void turnOffAllLeds() {
  Serial.println("Turning off all leds...");
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    turnOff(leds[i]);
  }
}

void cycleThroughLeds() {
  Serial.println("Cycling through all leds...");
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    turnOn(leds[i]);
    delay(200);
    turnOff(leds[i]);
  }
}

void toggleOn(int pin) {
  turnOffAllLeds();

  Serial.println("Toggling on led...");
  
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    if(pin == leds[i]) {
      turnOn(leds[i]);
    }
  }
}

void turnOn(int pin) {
  // Serial.println("Turning on led...");
  digitalWrite(pin, HIGH);
}

void turnOff(int pin) {
  // Serial.println("Turning off led...");
  digitalWrite(pin, LOW);
}

void turnLeft(int speed) {
  isMoving = true;
  Serial.println("Turning left...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.forward(actualSpeed);
  motorRight.back(actualSpeed);
}

void turnRight(int speed) {
  isMoving = true;
  Serial.println("Turning right...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.back(actualSpeed);
  motorRight.forward(actualSpeed);
}

void moveBack(int speed) {
  isMoving = true;
  Serial.println("Reversing...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.back(actualSpeed);
  motorRight.back(actualSpeed);
}

void moveForward(int speed) {
  isMoving = true;
  Serial.println("Moving forward...");
  int actualSpeed = convertSpeed(speed);
  Serial.println(actualSpeed);
  motorLeft.forward(actualSpeed);
  motorRight.forward(actualSpeed);
}

void stop() {
  isMoving = false;
  Serial.println("Stopping...");
  motorLeft.stop();
  motorRight.stop();
}

int convertSpeed(int speed) {
  if (speed == 0) {
    return 0;
  }
  return map(speed, 1, 5, 150, 255);
}
