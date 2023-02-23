#include "main.h"

void setup()
{
  Serial.begin(9600);

  bumperModule->begin();

  sensorModule->begin();

  pinMode(RECEIVE_PIN, INPUT);
  pinMode(TRANSMIT_PIN, OUTPUT);
  bluetoothSerial.begin(9600);

  commandListener.when("stp", (EvtCommandAction)stop);
  commandListener.when("fwd", (EvtCommandAction)forward);
  commandListener.when("rev", (EvtCommandAction)reverse);
  commandListener.when("lft", (EvtCommandAction)left);
  commandListener.when("rht", (EvtCommandAction)right);
  commandListener.when("blf", (EvtCommandAction)bearLeftForward);
  commandListener.when("brf", (EvtCommandAction)bearRightForward);
  commandListener.when("blr", (EvtCommandAction)bearLeftReverse);
  commandListener.when("brr", (EvtCommandAction)bearRightReverse);
  commandListener.when("spd", (EvtCommandAction)setSpeed);
  commandListener.when("aon", (EvtCommandAction)enableAutoPilot);
  commandListener.when("aof", (EvtCommandAction)disableAutoPilot);
  mgr.addListener(&commandListener);

  // autoPilotModule.enable();
  
  Serial.println(F("Setup complete, continuing..."));
}

void loop()
{
  mgr.loopIteration();

  selfDriveIfEnabled();
}

void selfDriveIfEnabled()
{
  autoPilotModule.handle();
}

bool enableAutoPilot()
{
  autoPilotModule.enable();
  return true;
}

bool disableAutoPilot()
{
  autoPilotModule.disable();
  drivingModule->stop();
  return true;
}

bool stop()
{
  drivingModule->stop();
  return true;
}

bool forward()
{
  drivingModule->moveForward();
  return true;
}

bool reverse()
{
  drivingModule->moveBackward();
  return true;
}

bool left()
{
  drivingModule->turnLeft();
  return true;
}

bool right()
{
  drivingModule->turnRight();
  return true;
}

bool bearLeftForward()
{
  drivingModule->bearLeft(DIR_FORWARD);
  return true;
}

bool bearRightForward()
{
  drivingModule->bearRight(DIR_FORWARD);
  return true;
}

bool bearLeftReverse()
{
  drivingModule->bearLeft(DIR_REVERSE);
  return true;
}

bool bearRightReverse()
{
  drivingModule->bearRight(DIR_REVERSE);
  return true;
}

bool setSpeed(EvtListener *, EvtContext *, long data)
{
  drivingModule->setSpeed((byte)data);
  return true;
}