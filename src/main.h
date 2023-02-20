#define EVENTUALLY_MAX_COMMANDS 12

#include <Arduino.h>
#include "IDrivingModule.h"
#include "DrivingModule.h"
#include "BumperModule.h"
#include "SensorModule.h"
#include "AutoPilotModule.h"
#include "Debug.h"
#include <Eventually.h>
#include <EventuallyCommand.h>
#include <SoftwareSerial.h>

// #define BOAT

const byte TRANSMIT_PIN = 2;
const byte RECEIVE_PIN = 3;

const byte MOTOR_LEFT_ENABLE_PIN = 6;
const byte MOTOR_LEFT_DIRECTION_PIN = 7;

const byte MOTOR_RIGHT_ENABLE_PIN = 11;
const byte MOTOR_RIGHT_DIRECTION_PIN = 8;

const byte LEFT_BUMPER_PIN = 10;
const byte RIGHT_BUMPER_PIN = 12;

const byte SENSOR_SERVO_PIN = 5;

BumperModule bumperModule(LEFT_BUMPER_PIN, RIGHT_BUMPER_PIN, &Serial);

SensorModule sm((byte)0x30, SENSOR_SERVO_PIN, &Serial);

SoftwareSerial bluetoothSerial(RECEIVE_PIN, TRANSMIT_PIN);

DrivingModule dm(MOTOR_LEFT_ENABLE_PIN, MOTOR_LEFT_DIRECTION_PIN,
                 MOTOR_RIGHT_ENABLE_PIN, MOTOR_RIGHT_DIRECTION_PIN,
                 &Serial);

#ifdef BOAT
#include "RudderModule.h"
#include "BoatDrivingModule.h"
const byte rudderPin = 2;
RudderModule rm(rudderPin, &Serial);
BoatDrivingModule bdm(&dm, &rm);
IDrivingModule *drivingModule = &bdm;
#else
SensorResult sensorResult;
IDrivingModule *drivingModule = &dm;
ISensorModule *sensorModule = &sm;
AutoPilotModule autoPilotModule(&Serial, drivingModule, sensorModule, &sensorResult);
#endif

EvtManager mgr;
// EvtCommandListener commandListener(&bluetoothSerial, 2);
EvtCommandListener commandListener(&Serial, 2);

bool stop();
bool forward();
bool reverse();
bool left();
bool right();
bool bearLeftForward();
bool bearRightForward();
bool bearLeftReverse();
bool bearRightReverse();
bool setSpeed(EvtListener *, EvtContext *, long data);

void selfDriveIfEnabled();
bool enableAutoPilot();
bool disableAutoPilot();

bool handleBumperEvent();