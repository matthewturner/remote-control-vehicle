#include <L293.h>

#define DEBUG_ON 1
#define DEBUG_OFF 0
const byte debugMode = DEBUG_OFF;
#define DBG(...) debugMode == DEBUG_ON ? Serial.println(__VA_ARGS__) : NULL
#define DBG_PRNT(...) debugMode == DEBUG_ON ? Serial.print(__VA_ARGS__) : NULL
#define DBG_WRT(...) debugMode == DEBUG_ON ? Serial.write(__VA_ARGS__) : NULL

#define DIR_FORWARD true
#define DIR_REVERSE false

const int REPLAY_DELAY = 700;
const int DEFAULT_EDGE_DURATION = 200;

const byte NUMBER_OF_LEDS = 3;
const byte ledRedPin = 12;
const byte ledGreenPin = 10;
const byte ledBluePin = 11;
byte leds[] = { ledGreenPin, ledRedPin, ledBluePin };

const byte ledOnboardPin = 13;
const byte motorLeftEnablePin = 9;
const byte motorLeftForwardPin = 7;
const byte motorLeftReversePin = 8;
const byte motorRightEnablePin = 3;
const byte motorRightForwardPin = 6;
const byte motorRightReversePin = 5;

L293 motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin);
L293 motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin);

bool isMoving = false;
bool edgeMode = false;
int edgeDuration = DEFAULT_EDGE_DURATION;
bool recordMode = false;
int currentInstruction = -1;
int instructions[300];
unsigned long timeOfPreviousInstruction;
bool awaitingDelayInstruction = false;
char commandBuffer[30];
byte currentSpeed = 5;

const int LEFT = 0xFF22DD;
const int RIGHT = 0xFFC23D;
const int FORWARD = 0xFF906F;
const int REVERSE = 0xFFE01F;
const int EDGE_LEFT = 0xFF22DC;
const int EDGE_RIGHT = 0xFFC23C;
const int EDGE_FORWARD = 0xFF906E;
const int EDGE_REVERSE = 0xFFE01E;
const int BEAR_LEFT_FORWARD = 0xFF22DB;
const int BEAR_RIGHT_FORWARD = 0xFFC23B;
const int BEAR_LEFT_REVERSE = 0xFF906D;
const int BEAR_RIGHT_REVERSE = 0xFFE01D;
const int STOP = 0;
const int REPLAY = 0xFF6897;
const int INCREASE_EDGE_DURATION = 0xFF629D;
const int DECREASE_EDGE_DURATION = 0xFFA857;
const int TOGGLE_RECORD_MODE = 0xFFB04F;
const int TOGGLE_EDGE_MODE = 0xFF9867;
const int DELAY = 0xAAAAAA;
const int PLAY_PAUSE = 0xFF02FD;
const int SET_SPEED_1 = 0xAAAAAB;
const int SET_SPEED_2 = 0xAAAAAC;
const int SET_SPEED_3 = 0xAAAAAD;
const int SET_SPEED_4 = 0xAAAAAE;
const int SET_SPEED_5 = 0xAAAAAF;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledOnboardPin, OUTPUT);

  turnOffAllLeds();
  cycleThroughLeds();
}

void loop() {
  int commandLength = tryReadCommand();
  
  if (commandLength > 0) {
    DBG("New command:");
    DBG_WRT(commandBuffer, commandLength);
    DBG();
    int instruction = convertToInstruction(commandLength);
    DBG("New instruction:");
    DBG(instruction);
    if (instruction != -1) {
      executeInstruction(instruction);
    }
  }
}

int tryReadCommand() {
  int index = -1;
  while (Serial.available()) {
    delay(2);
    char ch = Serial.read();
    switch (ch) {
      case '>':
        index = 0;
        break;
      case '!':
        commandBuffer[index] = '\0';
        return index;
      default:
        commandBuffer[index] = ch;
        index++;
        break;
    }
  }
  return -1;
}

int convertToInstruction(int commandLength) {
  if (strcmp(commandBuffer, "stop") == 0) {
    return STOP;
  }
  if (strcmp(commandBuffer, "forward") == 0) {
    return FORWARD;
  }
  if (strcmp(commandBuffer, "reverse") == 0) {
    return REVERSE;
  }
  if (strcmp(commandBuffer, "left") == 0) {
    return LEFT;
  }
  if (strcmp(commandBuffer, "right") == 0) {
    return RIGHT;
  }
  if (strcmp(commandBuffer, "bear-left-forward") == 0) {
    return BEAR_LEFT_FORWARD;
  }
  if (strcmp(commandBuffer, "bear-right-forward") == 0) {
    return BEAR_RIGHT_FORWARD;
  }
  if (strcmp(commandBuffer, "bear-left-reverse") == 0) {
    return BEAR_LEFT_REVERSE;
  }
  if (strcmp(commandBuffer, "bear-right-reverse") == 0) {
    return BEAR_RIGHT_REVERSE;
  }
  if (strcmp(commandBuffer, "set-speed-1") == 0) {
    return SET_SPEED_1;
  }
  if (strcmp(commandBuffer, "set-speed-2") == 0) {
    return SET_SPEED_2;
  }
  if (strcmp(commandBuffer, "set-speed-3") == 0) {
    return SET_SPEED_3;
  }
  if (strcmp(commandBuffer, "set-speed-4") == 0) {
    return SET_SPEED_4;
  }
  if (strcmp(commandBuffer, "set-speed-5") == 0) {
    return SET_SPEED_5;
  }
  if (strcmp(commandBuffer, "edge-reverse") == 0) {
    return EDGE_REVERSE;
  }
  if (strcmp(commandBuffer, "edge-left") == 0) {
    return EDGE_LEFT;
  }
  if (strcmp(commandBuffer, "edge-right") == 0) {
    return EDGE_RIGHT;
  }
  if (strcmp(commandBuffer, "edge-forward") == 0) {
    return EDGE_FORWARD;
  }
  if (strcmp(commandBuffer, "replay") == 0) {
    return REPLAY;
  }
  if (strcmp(commandBuffer, "increase-edge-duration") == 0) {
    return INCREASE_EDGE_DURATION;
  }
  if (strcmp(commandBuffer, "decrease-edge-duration") == 0) {
    return DECREASE_EDGE_DURATION;
  }
  if (strcmp(commandBuffer, "start-record") == 0) {
    if (recordMode) {
      return -1;
    } else {
      return TOGGLE_RECORD_MODE;
    }
  }
  if (strcmp(commandBuffer, "stop-record") == 0) {
    if (recordMode) {
      return TOGGLE_RECORD_MODE;
    } else {
      return -1;
    }
  }
  if (strcmp(commandBuffer, "start-edge") == 0) {
    if (edgeMode) {
      return -1;
    } else {
      return TOGGLE_EDGE_MODE;
    }
  }
  if (strcmp(commandBuffer, "stop-edge") == 0) {
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
        moveForward(currentSpeed);
        edgeIfRequired();
      }
      break;
    case FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveForward(currentSpeed);
      edgeIfRequired();
      break;
    case RIGHT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnRight(currentSpeed);
      edgeIfRequired();
      break;
    case LEFT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnLeft(currentSpeed);
      edgeIfRequired();
      break;
    case REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveBackward(currentSpeed);
      edgeIfRequired();
      break;
    case EDGE_FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveForward(currentSpeed);
      edge();
      break;
    case EDGE_RIGHT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnRight(currentSpeed);
      edge();
      break;
    case EDGE_LEFT:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      turnLeft(currentSpeed);
      edge();
      break;
    case EDGE_REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      moveBackward(currentSpeed);
      edge();
      break;
    case BEAR_LEFT_FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      bearLeft(currentSpeed, DIR_FORWARD);
      break;
    case BEAR_RIGHT_FORWARD:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      bearRight(currentSpeed, DIR_FORWARD);
      break;
    case BEAR_LEFT_REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      bearLeft(currentSpeed, DIR_REVERSE);
      break;
    case BEAR_RIGHT_REVERSE:
      recordDurationIfRequired();
      recordInstructionIfRequired(instruction);
      bearRight(currentSpeed, DIR_REVERSE);
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
        DBG_PRNT("Waiting for ");
        DBG_PRNT(instruction);
        DBG("ms...");
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
  resetEdgeMode();
  DBG("Replaying instructions...");
  for(int i = 0; i < currentInstruction; i++) {
    executeInstruction(instructions[i]);
    if (instructions[i] != DELAY && instructions[i + 1] != DELAY) {
      delay(REPLAY_DELAY);
    }
  }
}

void toggleRecordMode() {
  DBG("Toggling record mode...");
  turnRecordMode(!recordMode);
}

void turnRecordMode(bool on) {
  recordMode = on;
  if (recordMode) {
    toggleOn(ledRedPin);
    currentInstruction = 0;
    resetEdgeMode();
    currentSpeed = 5;
  } else {
    turnOff(ledRedPin);
    resetEdgeMode();
    currentSpeed = 5;
  }
}

void resetEdgeMode() {
  edgeDuration = DEFAULT_EDGE_DURATION;
  turnEdgeMode(false);
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
    DBG_PRNT("Recording instruction ");
    DBG_PRNT(instruction, HEX);
    DBG("...");
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
  // DBG("Turning off all leds...");
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    turnOff(leds[i]);
  }
}

void cycleThroughLeds() {
  DBG("Cycling through all leds...");
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    turnOn(leds[i]);
    delay(200);
    turnOff(leds[i]);
  }
}

void toggleOn(byte pin) {
  turnOffAllLeds();

  // DBG("Toggling on led...");
  
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    if(pin == leds[i]) {
      turnOn(leds[i]);
    }
  }
}

void turnOn(byte pin) {
  // DBG("Turning on led...");
  digitalWrite(pin, HIGH);
}

void turnOff(byte pin) {
  // DBG("Turning off led...");
  digitalWrite(pin, LOW);
}

void bearLeft(byte speed, bool forward) {
  isMoving = true;
  DBG("Bearing left...");
  byte actualSpeed = convertSpeed(speed);
  byte reducedSpeed = 0;
  if (speed > 4) {
    reducedSpeed = convertSpeed(speed - 4);
  }
  if (forward) {
    motorLeft.forward(reducedSpeed);
    motorRight.forward(actualSpeed);
  } else {
    motorLeft.back(reducedSpeed);
    motorRight.back(actualSpeed);
  }
}

void bearRight(byte speed, bool forward) {
  isMoving = true;
  DBG("Bearing left...");
  byte actualSpeed = convertSpeed(speed);
  byte reducedSpeed = 0;
  if (speed > 4) {
    reducedSpeed = convertSpeed(speed - 4);
  }
  if (forward) {
    motorLeft.forward(actualSpeed);
    motorRight.forward(reducedSpeed);
  } else {
    motorLeft.back(actualSpeed);
    motorRight.back(reducedSpeed);
  }
}

void turnLeft(byte speed) {
  isMoving = true;
  DBG("Turning left...");
  byte actualSpeed = convertSpeed(speed);
  motorLeft.back(actualSpeed);
  motorRight.forward(actualSpeed);
}

void turnRight(byte speed) {
  isMoving = true;
  DBG("Turning right...");
  byte actualSpeed = convertSpeed(speed);
  motorLeft.forward(actualSpeed);
  motorRight.back(actualSpeed);
}

void moveBackward(byte speed) {
  isMoving = true;
  DBG("Reversing...");
  byte actualSpeed = convertSpeed(speed);
  motorLeft.back(actualSpeed);
  motorRight.back(actualSpeed);
}

void moveForward(byte speed) {
  isMoving = true;
  DBG("Moving forward...");
  byte actualSpeed = convertSpeed(speed);
  // DBG(actualSpeed);
  motorLeft.forward(actualSpeed);
  motorRight.forward(actualSpeed);
}

void stop() {
  isMoving = false;
  DBG("Stopping...");
  motorLeft.stop();
  motorRight.stop();
}

byte convertSpeed(byte speed) {
  if (speed == 0) {
    return 0;
  }
  return map(speed, 1, 5, 50, 255);
}
