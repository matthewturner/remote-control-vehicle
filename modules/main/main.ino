#include "DrivingModule.h"
#include "SensorModule.h"
#include "LedModule.h"
#include "CommandModule.h"
#include "RecordModule.h"
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

CommandModule commandModule;
RecordModule recordModule(REPLAY_DELAY);

int edgeDuration = DEFAULT_EDGE_DURATION;
bool awaitingDelayInstruction = false;
byte currentSpeed = 5;

void setup()
{
  Serial.begin(9600);
  sensorModule.begin();

  ledModule.turnOffAllLeds();
  ledModule.cycleThroughLeds();
}

void loop()
{
  int instruction = commandModule.tryReadInstruction();
  if (instruction != -1)
  {
    executeInstruction(instruction);
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
  if (!commandModule.recordMode())
  {
    return;
  }

  unsigned long time = millis();
  if (drivingModule.isMoving())
  {
    int duration = recordModule.timeSinceLastInstruction(time);
    recordModule.recordInstruction(0xAAAAAA);
    recordModule.recordInstruction(duration);
  }
  recordModule.setTimeOfLastInstruction(time);
}

void replayInstructions()
{
  resetEdgeMode();
  DBG("Replaying instructions...");
  recordModule.replay();
  int instruction;
  while (instruction = recordModule.next() != -1)
  {
    executeInstruction(instruction);
    if (instruction != DELAY && recordModule.peek() != DELAY)
    {
      delay(REPLAY_DELAY);
    }
  }
}

void toggleRecordMode()
{
  DBG("Toggling record mode...");
  turnRecordMode(!commandModule.recordMode());
}

void turnRecordMode(bool on)
{
  commandModule.turnRecordMode(on);
  if (commandModule.recordMode())
  {
    ledModule.toggleOn(ledRedPin);
    recordModule.reset();
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
  if (commandModule.recordMode())
  {
    recordModule.recordInstruction(instruction);
  }
}

void toggleEdgeMode()
{
  turnEdgeMode(!commandModule.edgeMode());
}

void turnEdgeMode(bool on)
{
  commandModule.turnEdgeMode(on);
  if (commandModule.edgeMode())
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
  if (commandModule.edgeMode())
  {
    edge();
  }
}

void edge()
{
  delay(edgeDuration);
  drivingModule.stop();
}
