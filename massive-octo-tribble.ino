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

const int DEFAULT_EDGE_DURATION = 200;
bool isMoving = false;
bool edgeMode = false;
int edgeDuration = DEFAULT_EDGE_DURATION;
bool recordMode = false;
int currentInstruction = -1;
int instructions[300];
bool recordInstruction = true;

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
  if (!irrecv.decode(&results)) {
    return;
  }

  Serial.println(results.value, HEX);
  irrecv.resume();
  executeInstruction(results.value);
}

void executeInstruction(int instruction) {
  recordInstruction = true;
  switch(instruction) {
    case 0xFF906F:
      moveForward(5);
      break;
    case 0xFF02FD:
      if (isMoving) {
        stop();
      } else {
        moveForward(5);
      }
      break;
    case 0xFFC23D:
      turnRight(5);
      break;
    case 0xFF22DD:
      turnLeft(5);
      break;
    case 0xFFE01F:
      moveBackward(5);
      break;
    case 0xFF9867:
      toggleEdgeMode();
      break;
    case 0xFF629D:
      increaseEdgeDuration();
      break;
    case 0xFFA857:
      decreaseEdgeDuration();
      break;
    case 0xFFB04F:
      toggleRecordMode();
      recordInstruction = false;
      break;
    case 0xFF6897:
      replayInstructions();
      recordInstruction = false;
      break;
    case 0:
      stop();
      break;
  }
  recordInstructionIfRequired(instruction);
}

void replayInstructions() {
  Serial.println("Replaying instructions...");
  for(int i = 0; i <= currentInstruction; i++) {
    executeInstruction(instructions[i]);
    delay(150);
  }
}

void toggleRecordMode() {
  Serial.println("Toggling record mode...");
  recordMode = !recordMode;
  if (recordMode) {
    toggleOn(ledRedPin);
    currentInstruction = 0;
    edgeDuration = DEFAULT_EDGE_DURATION;
    turnEdgeMode(false);
  } else {
    turnOff(ledRedPin);
    edgeDuration = DEFAULT_EDGE_DURATION;
    turnEdgeMode(false);
  }
}

void recordInstructionIfRequired(int instruction) {
  if (recordMode && recordInstruction) {
    Serial.print("Recording instruction ");
    Serial.print(instruction);
    Serial.println("...");
    instructions[currentInstruction] = instruction;
    currentInstruction++;
  }
}

void toggleEdgeMode() {
  turnEdgeMode(!edgeMode);
}

void turnEdgeMode(bool on) {
  edgeMode = on;
  if (edgeMode) {
    turnOn(ledBluePin);
  } else {
    turnOff(ledBluePin);
  }
}

void increaseEdgeDuration() {
  edgeDuration += 100;
  if (edgeDuration >= 500) {
    edgeDuration = 500;
  }
}

void decreaseEdgeDuration() {
  edgeDuration -= 100;
  if (edgeDuration <= 100) {
    edgeDuration = 100;
  }
}

void edgeIfRequired() {
  if (edgeMode) {
    delay(edgeDuration);
    stop();
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
  motorLeft.back(actualSpeed);
  motorRight.forward(actualSpeed);
  edgeIfRequired();
}

void turnRight(int speed) {
  isMoving = true;
  Serial.println("Turning right...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.forward(actualSpeed);
  motorRight.back(actualSpeed);
  edgeIfRequired();
}

void moveBackward(int speed) {
  isMoving = true;
  Serial.println("Reversing...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.back(actualSpeed);
  motorRight.back(actualSpeed);
  edgeIfRequired();
}

void moveForward(int speed) {
  isMoving = true;
  Serial.println("Moving forward...");
  int actualSpeed = convertSpeed(speed);
  Serial.println(actualSpeed);
  motorLeft.forward(actualSpeed);
  motorRight.forward(actualSpeed);
  edgeIfRequired();
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
