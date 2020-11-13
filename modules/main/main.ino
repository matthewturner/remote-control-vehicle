#include "DrivingModule.h"
#include "SensorModule.h"
#include "Debug.h"

#define REPLAY_DELAY 700
#define DEFAULT_EDGE_DURATION 200
#define SENSOR_I2C_ADDR 8

const byte ledRedPin = 12;
const byte ledGreenPin = 10;
const byte ledBluePin = 11;
LedModule ledModule(ledRedPin, ledGreenPin, ledBluePin);

const byte motorLeftEnablePin = 9;
const byte motorLeftForwardPin = 7;
const byte motorLeftReversePin = 8;
const byte motorRightEnablePin = 3;
const byte motorRightForwardPin = 6;
const byte motorRightReversePin = 5;

DrivingModule drivingModule(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin,
                            motorRightEnablePin, motorRightForwardPin, motorRightReversePin);

SensorModule sensorModule(SENSOR_I2C_ADDR);
SensorResult sensorResult;

bool edgeMode = false;
int edgeDuration = DEFAULT_EDGE_DURATION;
bool recordMode = false;
int currentInstruction = -1;
int instructions[300];
unsigned long timeOfPreviousInstruction;
bool awaitingDelayInstruction = false;
char commandBuffer[30];
byte currentSpeed = 5;
bool selfDriveMode = false;

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
const int TOGGLE_SELF_DRIVE_MODE = 0xFF9868;
const int DELAY = 0xAAAAAA;
const int PLAY_PAUSE = 0xFF02FD;
const int SET_SPEED_1 = 0xAAAAAB;
const int SET_SPEED_2 = 0xAAAAAC;
const int SET_SPEED_3 = 0xAAAAAD;
const int SET_SPEED_4 = 0xAAAAAE;
const int SET_SPEED_5 = 0xAAAAAF;

void setup()
{
  Serial.begin(9600);
  sensorModule.begin();

  ledModule.turnOffAllLeds();
  ledModule.cycleThroughLeds();
}

void loop()
{
  int commandLength = tryReadCommand();

  if (commandLength > 0)
  {
    DBG("New command:");
    DBG_WRT(commandBuffer, commandLength);
    DBG();
    int instruction = convertToInstruction(commandLength);
    DBG("New instruction:");
    DBG(instruction);
    if (instruction != -1)
    {
      executeInstruction(instruction);
    }
  }

  sensorModule.detect(&sensorResult);
  if (sensorResult.Age > 100)
  {
    drivingModule.stop();
  }
  Serial.println("Sensor Module Result:");
  Serial.print("   ");
  Serial.println(sensorResult.Forward);
  Serial.print(sensorResult.Left);
  Serial.print("       ");
  Serial.println(sensorResult.Right);
  Serial.print("   ");
  Serial.println(sensorResult.Back);
  Serial.print("   ~");
  Serial.println(sensorResult.Age);
}

int tryReadCommand()
{
  int index = -1;
  while (Serial.available())
  {
    delay(2);
    char ch = Serial.read();
    switch (ch)
    {
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

int convertToInstruction(int commandLength)
{
  if (strcmp(commandBuffer, "stop") == 0)
  {
    return STOP;
  }
  if (strcmp(commandBuffer, "forward") == 0)
  {
    return FORWARD;
  }
  if (strcmp(commandBuffer, "reverse") == 0)
  {
    return REVERSE;
  }
  if (strcmp(commandBuffer, "left") == 0)
  {
    return LEFT;
  }
  if (strcmp(commandBuffer, "right") == 0)
  {
    return RIGHT;
  }
  if (strcmp(commandBuffer, "bear-left-forward") == 0)
  {
    return BEAR_LEFT_FORWARD;
  }
  if (strcmp(commandBuffer, "bear-right-forward") == 0)
  {
    return BEAR_RIGHT_FORWARD;
  }
  if (strcmp(commandBuffer, "bear-left-reverse") == 0)
  {
    return BEAR_LEFT_REVERSE;
  }
  if (strcmp(commandBuffer, "bear-right-reverse") == 0)
  {
    return BEAR_RIGHT_REVERSE;
  }
  if (strcmp(commandBuffer, "set-speed-1") == 0)
  {
    return SET_SPEED_1;
  }
  if (strcmp(commandBuffer, "set-speed-2") == 0)
  {
    return SET_SPEED_2;
  }
  if (strcmp(commandBuffer, "set-speed-3") == 0)
  {
    return SET_SPEED_3;
  }
  if (strcmp(commandBuffer, "set-speed-4") == 0)
  {
    return SET_SPEED_4;
  }
  if (strcmp(commandBuffer, "set-speed-5") == 0)
  {
    return SET_SPEED_5;
  }
  if (strcmp(commandBuffer, "edge-reverse") == 0)
  {
    return EDGE_REVERSE;
  }
  if (strcmp(commandBuffer, "edge-left") == 0)
  {
    return EDGE_LEFT;
  }
  if (strcmp(commandBuffer, "edge-right") == 0)
  {
    return EDGE_RIGHT;
  }
  if (strcmp(commandBuffer, "edge-forward") == 0)
  {
    return EDGE_FORWARD;
  }
  if (strcmp(commandBuffer, "start-self-drive") == 0)
  {
    if (selfDriveMode)
    {
      return -1;
    }
    else
    {
      return TOGGLE_SELF_DRIVE_MODE;
    }
  }
  if (strcmp(commandBuffer, "stop-self-drive") == 0)
  {
    if (selfDriveMode)
    {
      return TOGGLE_SELF_DRIVE_MODE;
    }
    else
    {
      return -1;
    }
  }
  if (strcmp(commandBuffer, "replay") == 0)
  {
    return REPLAY;
  }
  if (strcmp(commandBuffer, "increase-edge-duration") == 0)
  {
    return INCREASE_EDGE_DURATION;
  }
  if (strcmp(commandBuffer, "decrease-edge-duration") == 0)
  {
    return DECREASE_EDGE_DURATION;
  }
  if (strcmp(commandBuffer, "start-record") == 0)
  {
    if (recordMode)
    {
      return -1;
    }
    else
    {
      return TOGGLE_RECORD_MODE;
    }
  }
  if (strcmp(commandBuffer, "stop-record") == 0)
  {
    if (recordMode)
    {
      return TOGGLE_RECORD_MODE;
    }
    else
    {
      return -1;
    }
  }
  if (strcmp(commandBuffer, "start-edge") == 0)
  {
    if (edgeMode)
    {
      return -1;
    }
    else
    {
      return TOGGLE_EDGE_MODE;
    }
  }
  if (strcmp(commandBuffer, "stop-edge") == 0)
  {
    if (edgeMode)
    {
      return TOGGLE_EDGE_MODE;
    }
    else
    {
      return -1;
    }
  }
  return -1;
}

void executeInstruction(int instruction)
{
  switch (instruction)
  {
  case DELAY:
    awaitingDelayInstruction = true;
    break;
  case PLAY_PAUSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    if (drivingModule.isMoving())
    {
      drivingModule.stop();
    }
    else
    {
      drivingModule.moveForward(currentSpeed);
      edgeIfRequired();
    }
    break;
  case SET_SPEED_1:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    currentSpeed = 1;
    break;
  case SET_SPEED_2:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    currentSpeed = 2;
    break;
  case SET_SPEED_3:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    currentSpeed = 3;
    break;
  case SET_SPEED_4:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    currentSpeed = 4;
    break;
  case SET_SPEED_5:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    currentSpeed = 5;
    break;
  case FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveForward(currentSpeed);
    edgeIfRequired();
    break;
  case RIGHT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnRight(currentSpeed);
    edgeIfRequired();
    break;
  case LEFT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnLeft(currentSpeed);
    edgeIfRequired();
    break;
  case REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveBackward(currentSpeed);
    edgeIfRequired();
    break;
  case EDGE_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveForward(currentSpeed);
    edge();
    break;
  case EDGE_RIGHT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnRight(currentSpeed);
    edge();
    break;
  case EDGE_LEFT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnLeft(currentSpeed);
    edge();
    break;
  case EDGE_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveBackward(currentSpeed);
    edge();
    break;
  case BEAR_LEFT_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearLeft(currentSpeed, DIR_FORWARD);
    break;
  case BEAR_RIGHT_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearRight(currentSpeed, DIR_FORWARD);
    break;
  case BEAR_LEFT_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearLeft(currentSpeed, DIR_REVERSE);
    break;
  case BEAR_RIGHT_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearRight(currentSpeed, DIR_REVERSE);
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
    drivingModule.stop();
    break;
  default:
    if (awaitingDelayInstruction)
    {
      awaitingDelayInstruction = false;
      DBG_PRNT("Waiting for ");
      DBG_PRNT(instruction);
      DBG("ms...");
      delay(instruction);
    }
  }
}

void recordDurationIfRequired()
{
  if (!recordMode)
  {
    return;
  }

  unsigned long time = millis();
  if (drivingModule.isMoving())
  {
    int duration = time - timeOfPreviousInstruction;
    recordInstructionIfRequired(0xAAAAAA);
    recordInstructionIfRequired(duration);
  }
  timeOfPreviousInstruction = time;
}

void replayInstructions()
{
  resetEdgeMode();
  DBG("Replaying instructions...");
  for (int i = 0; i < currentInstruction; i++)
  {
    executeInstruction(instructions[i]);
    if (instructions[i] != DELAY && instructions[i + 1] != DELAY)
    {
      delay(REPLAY_DELAY);
    }
  }
}

void toggleRecordMode()
{
  DBG("Toggling record mode...");
  turnRecordMode(!recordMode);
}

void turnRecordMode(bool on)
{
  recordMode = on;
  if (recordMode)
  {
    ledModule.toggleOn(ledRedPin);
    currentInstruction = 0;
    resetEdgeMode();
    currentSpeed = 5;
  }
  else
  {
    ledModule.turnOff(ledRedPin);
    resetEdgeMode();
    currentSpeed = 5;
  }
}

void resetEdgeMode()
{
  edgeDuration = DEFAULT_EDGE_DURATION;
  turnEdgeMode(false);
}

void recordInstructionIfRequired(int instruction)
{
  if (recordMode)
  {
    if (instruction == 0)
    {
      if (currentInstruction > 0)
      {
        if (instructions[currentInstruction - 1] == 0)
        {
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

void toggleEdgeMode()
{
  turnEdgeMode(!edgeMode);
}

void turnEdgeMode(bool on)
{
  edgeMode = on;
  if (edgeMode)
  {
    ledModule.turnOn(ledBluePin);
  }
  else
  {
    ledModule.turnOff(ledBluePin);
  }
}

void increaseEdgeDuration()
{
  edgeDuration += 100;
  if (edgeDuration >= 500)
  {
    edgeDuration = 500;
  }
}

void decreaseEdgeDuration()
{
  edgeDuration -= 100;
  if (edgeDuration <= 100)
  {
    edgeDuration = 100;
  }
}

void edgeIfRequired()
{
  if (edgeMode)
  {
    edge();
  }
}

void edge()
{
  delay(edgeDuration);
  drivingModule.stop();
}
