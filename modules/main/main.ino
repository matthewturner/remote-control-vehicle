#include "DrivingModule.h"
#include "SensorModule.h"
#include "LedModule.h"
#include "CommandModule.h"
#include "RecordModule.h"
#include "EdgeModule.h"
#include "Debug.h"
#include "ControlModule.h"

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
                            motorRightEnablePin, motorRightForwardPin, motorRightReversePin,
                            &Serial);

SensorModule sensorModule(SENSOR_I2C_ADDR, &Serial);
SensorResult sensorResult;

CommandModule commandModule(&Serial);
EdgeModule edgeModule(&Serial);
RecordModule recordModule(REPLAY_DELAY, &Serial);

ControlModule controlModule(&Serial, &drivingModule, &recordModule,
                            &edgeModule, &ledModule, &commandModule);

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
