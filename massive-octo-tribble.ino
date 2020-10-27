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
unsigned long timeOfPreviousInstruction;
bool awaitingDelayInstruction = false;

const int LEFT = 0xFF22DD;
const int RIGHT = 0xFFC23D;
const int FORWARD = 0xFF906F;
const int REVERSE = 0xFFE01F;
const int EDGE_LEFT = 0xFF22DC;
const int EDGE_RIGHT = 0xFFC23C;
const int EDGE_FORWARD = 0xFF906E;
const int EDGE_REVERSE = 0xFFE01E;
const int STOP = 0;
const int REPLAY = 0xFF6897;
const int INCREASE_EDGE_DURATION = 0xFF629D;
const int DECREASE_EDGE_DURATION = 0xFFA857;
const int TOGGLE_RECORD_MODE = 0xFFB04F;
const int TOGGLE_EDGE_MODE = 0xFF9867;
const int DELAY = 0xAAAAAA;
const int PLAY_PAUSE = 0xFF02FD;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledOnboardPin, OUTPUT);

  irrecv.enableIRIn();

  turnOffAllLeds();
  cycleThroughLeds();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    Serial.println("New command:");
    Serial.println(command);
    int instruction = convertToInstruction(command);
    Serial.println("New instruction:");
    Serial.println(instruction);
    if (instruction != -1) {
      executeInstruction(instruction);
    }
    return;
  }
  
  if (irrecv.decode(&results)) {
    // Serial.println(results.value, HEX);
    irrecv.resume();
  
    executeInstruction(results.value);
    return;
  }
}

int convertToInstruction(String command) {
  if (command == "forward") {
    return FORWARD;
  } else if (command == "reverse") {
    return REVERSE;
  } else if (command == "left") {
    return LEFT;
  } else if (command == "right") {
    return RIGHT;
  } else if (command == "forward") {
    return FORWARD;
  } else if (command == "edge-reverse") {
    return EDGE_REVERSE;
  } else if (command == "edge-left") {
    return EDGE_LEFT;
  } else if (command == "edge-right") {
    return EDGE_RIGHT;
  } else if (command == "edge-forward") {
    return EDGE_FORWARD;
  } else if (command ==  "stop") {
    return STOP;
  } else if (command == "replay") {
    return REPLAY;
  } else if (command == "increase-edge-duration") {
    return INCREASE_EDGE_DURATION;
  } else if (command == "decrease-edge-duration") {
    return DECREASE_EDGE_DURATION;
  } else if (command == "start-record") {
    if (recordMode) {
      return -1;
    } else {
      return TOGGLE_RECORD_MODE;
    }
  } else if (command == "stop-record") {
    if (recordMode) {
      return TOGGLE_RECORD_MODE;
    } else {
      return -1;
    }
  } else if (command == "start-edge") {
    if (edgeMode) {
      return -1;
    } else {
      return TOGGLE_EDGE_MODE;
    }
  } else if (command == "stop-edge") {
    if (edgeMode) {
      return TOGGLE_EDGE_MODE;
    } else {
      return -1;
    }
  }
  return -1;
}

void executeInstruction(int instruction) {
  switch (instruction) {
    case DELAY:
      awaitingDelayInstruction = true;
      break;
    case PLAY_PAUSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      if (isMoving) {
        stop();
      } else {
        moveForward(5);
        edgeIfRequired();
      }
      break;
    case FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveForward(5);
      edgeIfRequired();
      break;
    case RIGHT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnRight(5);
      edgeIfRequired();
      break;
    case LEFT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnLeft(5);
      edgeIfRequired();
      break;
    case REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveBackward(5);
      edgeIfRequired();
      break;
    case EDGE_FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveForward(5);
      edge();
      break;
    case EDGE_RIGHT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnRight(5);
      edge();
      break;
    case EDGE_LEFT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnLeft(5);
      edge();
      break;
    case EDGE_REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveBackward(5);
      edge();
      break;
    case TOGGLE_EDGE_MODE:
      recordInstructionIfRequired(instruction);
      toggleEdgeMode();
      break;
    case INCREASE_EDGE_DURATION:
      recordInstructionIfRequired(instruction);
      increaseEdgeDuration();
      break;
    case DECREASE_EDGE_DURATION:
      recordInstructionIfRequired(instruction);
      decreaseEdgeDuration();
      break;
    case TOGGLE_RECORD_MODE:
      toggleRecordMode();
      break;
    case REPLAY:
      replayInstructions();
      break;
    case STOP:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      stop();
      break;
    default:
      if (awaitingDelayInstruction) {
        awaitingDelayInstruction = false;
        Serial.print("Waiting for ");
        Serial.print(instruction);
        Serial.println("ms...");
        delay(instruction);
      }
  }
}

void recordDurationIfRequired() {
  if (!recordMode) {
    return;
  }
  
  unsigned long time = millis();
  if (isMoving) {
    int duration = time - timeOfPreviousInstruction;
    recordInstructionIfRequired(0xAAAAAA);
    recordInstructionIfRequired(duration);
  }
  timeOfPreviousInstruction = time;
}

void replayInstructions() {
  Serial.println("Replaying instructions...");
  for(int i = 0; i <= currentInstruction; i++) {
    executeInstruction(instructions[i]);
    delay(200);
  }
}

void toggleRecordMode() {
  Serial.println("Toggling record mode...");
  turnRecordMode(!recordMode);
}

void turnRecordMode(bool on) {
  recordMode = on;
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
  if (recordMode) {
    if (instruction == 0) {
      if (currentInstruction > 0) {
        if (instructions[currentInstruction - 1] == 0) {
          return;
        }
      }
    }
    Serial.print("Recording instruction ");
    Serial.print(instruction, HEX);
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
    edge();
  }
}

void edge() {
  delay(edgeDuration);
  stop();
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
}

void turnRight(int speed) {
  isMoving = true;
  Serial.println("Turning right...");
  int actualSpeed = convertSpeed(speed);
  motorLeft.forward(actualSpeed);
  motorRight.back(actualSpeed);
}

void moveBackward(int speed) {
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
  // Serial.println(actualSpeed);
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
