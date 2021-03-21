#include "DrivingModule.h"
#include "SensorModule.h"
#include "LedModule.h"
#include "CommandModule.h"
#include "RecordModule.h"
#include "EdgeModule.h"
#include "Debug.h"
#include "ControlModule.h"
#include "AutoPilotModule.h"

#define CAR 0
#define BOAT 1
#define VEHICLE_TYPE CAR

#define SENSOR_I2C_ADDR 8

const byte sensorInterruptPin = 13;

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

RudderModule* rudderModule = NULL;
if (VEHICLE_TYPE == BOAT)
{
#include "RudderModule.h"
RudderModule rm(2, &Serial);
rudderModule = &rm;
}

DrivingModule drivingModule(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin,
                            motorRightEnablePin, motorRightForwardPin, motorRightReversePin,
                            &Serial, rudderModule);

SensorModule sensorModule(SENSOR_I2C_ADDR, sensorInterruptPin, &Serial);
SensorResult sensorResult;

CommandModule commandModule(&Serial);
EdgeModule edgeModule(&Serial);
RecordModule recordModule(REPLAY_DELAY, &Serial);

ControlModule controlModule(&Serial, &drivingModule, &recordModule,
                            &edgeModule, &ledModule, &commandModule);

AutoPilotModule autoPilotModule(&Serial, &drivingModule,
                                &commandModule, &sensorModule);

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
  controlModule.executeInstruction(instruction);

  autoPilotModule.handle();
}
