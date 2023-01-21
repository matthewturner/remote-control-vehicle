#define EVENTUALLY_MAX_COMMANDS 12

#include "IDrivingModule.h"
#include "DrivingModule.h"
#include "LedModule.h"
#include "Debug.h"
#include <Eventually.h>
#include <EventuallyCommand.h>
#include <SoftwareSerial.h>

// #define BOAT

const byte TRANSMIT_PIN = 2;
const byte RECEIVE_PIN = 3;
SoftwareSerial bluetoothSerial(RECEIVE_PIN, TRANSMIT_PIN);

const byte MOTOR_LEFT_ENABLE_PIN = 5;
const byte MOTOR_LEFT_FORWARD_PIN = 4;
const byte MOTOR_LEFT_REVERSE_PIN = 6;

const byte MOTOR_RIGHT_ENABLE_PIN = 10;
const byte MOTOR_RIGHT_FORWARD_PIN = 8;
const byte MOTOR_RIGHT_REVERSE_PIN = 7;

DrivingModule dm(MOTOR_LEFT_ENABLE_PIN, MOTOR_LEFT_FORWARD_PIN, MOTOR_LEFT_REVERSE_PIN,
                 MOTOR_RIGHT_ENABLE_PIN, MOTOR_RIGHT_FORWARD_PIN, MOTOR_RIGHT_REVERSE_PIN,
                 &Serial);

#ifdef BOAT
#include "RudderModule.h"
#include "BoatDrivingModule.h"
const byte rudderPin = 2;
RudderModule rm(rudderPin, &Serial);
BoatDrivingModule bdm(&dm, &rm);
IDrivingModule *drivingModule = &bdm;
#else
IDrivingModule *drivingModule = &dm;
#endif

EvtManager mgr;
EvtCommandListener commandListener(&bluetoothSerial, 2);
// EvtCommandListener commandListener(&Serial, 2);

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