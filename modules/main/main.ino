#include "DrivingModule.h"
#include "SensorModule.h"
#include "LedModule.h"
#include "CommandModule.h"
#include "RecordModule.h"
#include "EdgeModule.h"
#include "Debug.h"

#define REPLAY_DELAY 700
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

CommandModule commandModule(&Serial);
EdgeModule edgeModule;
RecordModule recordModule(REPLAY_DELAY);

bool awaitingDelayInstruction = false;

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

  // sensorModule.detect(&sensorResult);

  // Serial.println("Sensor Module Result:");
  // Serial.print("   ");
  // Serial.println(sensorResult.Front);
  // Serial.print(sensorResult.Left);
  // Serial.print("       ");
  // Serial.println(sensorResult.Right);
  // Serial.print("   ");
  // Serial.println(sensorResult.Back);
  // Serial.print("   ~");
  // Serial.println(sensorResult.Age);

  // if (sensorResult.Age > 100)
  // {
  //   drivingModule.stop();
  // }
  // if (sensorResult.Front < 10)
  // {
  //   drivingModule.stop();
  // }
}

void executeInstruction(int instruction)
{
  Serial.println(instruction);
  switch (instruction)
  {
  case DELAY:
    awaitingDelayInstruction = true;
    break;
  case SET_SPEED_1:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.setSpeed(1);
    break;
  case SET_SPEED_2:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.setSpeed(2);
    break;
  case SET_SPEED_3:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.setSpeed(3);
    break;
  case SET_SPEED_4:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.setSpeed(4);
    break;
  case SET_SPEED_5:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.setSpeed(5);
    break;
  case FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveForward();
    break;
  case RIGHT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnRight();
    break;
  case LEFT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnLeft();
    break;
  case REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveBackward();
    break;
  case EDGE_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveForward();
    Serial.println(edgeModule.duration());
    delay(edgeModule.duration());
    drivingModule.stop();
    break;
  case EDGE_RIGHT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnRight();
    delay(edgeModule.duration());
    drivingModule.stop();
    break;
  case EDGE_LEFT:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.turnLeft();
    delay(edgeModule.duration());
    drivingModule.stop();
    break;
  case EDGE_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.moveBackward();
    delay(edgeModule.duration());
    drivingModule.stop();
    break;
  case BEAR_LEFT_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearLeft(DIR_FORWARD);
    break;
  case BEAR_RIGHT_FORWARD:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearRight(DIR_FORWARD);
    break;
  case BEAR_LEFT_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearLeft(DIR_REVERSE);
    break;
  case BEAR_RIGHT_REVERSE:
    recordDurationIfRequired();
    recordInstructionIfRequired(instruction);
    drivingModule.bearRight(DIR_REVERSE);
    break;
  case INCREASE_EDGE_DURATION:
    recordInstructionIfRequired(instruction);
    edgeModule.increase();
    break;
  case DECREASE_EDGE_DURATION:
    recordInstructionIfRequired(instruction);
    edgeModule.decrease();
    break;
  case TOGGLE_RECORD_MODE:
    toggleRecordMode();
    break;
  case TOGGLE_SELF_DRIVE_MODE:
    toggleSelfDriveMode();
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
  edgeModule.reset();
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

void toggleSelfDriveMode()
{
  DBG("Toggling self-drive mode...");
  turnSelfDriveMode(!commandModule.selfDriveMode());
}

void turnSelfDriveMode(bool on)
{
  commandModule.turnSelfDriveMode(on);
  if (commandModule.selfDriveMode())
  {
    ledModule.toggleOn(ledGreenPin);
  }
  else
  {
    ledModule.turnOff(ledGreenPin);
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
    edgeModule.reset();
    drivingModule.resetSpeed();
  }
  else
  {
    ledModule.turnOff(ledRedPin);
    edgeModule.reset();
    drivingModule.resetSpeed();
  }
}

void recordInstructionIfRequired(int instruction)
{
  if (commandModule.recordMode())
  {
    recordModule.recordInstruction(instruction);
  }
}
