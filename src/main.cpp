#include "main.h"

void setup()
{
  Serial.begin(9600);

  bumperModule.begin();

  sensorModule->begin();

  pinMode(RECEIVE_PIN, INPUT);
  pinMode(TRANSMIT_PIN, OUTPUT);
  bluetoothSerial.begin(9600);

  commandListener.when("stop", (EvtCommandAction)stop);
  commandListener.when("forward", (EvtCommandAction)forward);
  commandListener.when("reverse", (EvtCommandAction)reverse);
  commandListener.when("left", (EvtCommandAction)left);
  commandListener.when("right", (EvtCommandAction)right);
  commandListener.when("bear-lf", (EvtCommandAction)bearLeftForward);
  commandListener.when("bear-rf", (EvtCommandAction)bearRightForward);
  commandListener.when("bear-lr", (EvtCommandAction)bearLeftReverse);
  commandListener.when("bear-rr", (EvtCommandAction)bearRightReverse);
  commandListener.when("set-speed", (EvtCommandAction)setSpeed);
  mgr.addListener(&commandListener);

  mgr.addListener(new EvtPinListener(LEFT_BUMPER_PIN, (EvtAction)handleBumperEvent));
  mgr.addListener(new EvtPinListener(RIGHT_BUMPER_PIN, (EvtAction)handleBumperEvent));

  Serial.println(F("Setup complete, continuing..."));
}

void loop()
{
  mgr.loopIteration();

  SensorResult sensorResult;
  sensorModule->scan(&sensorResult);
}

bool stop()
{
  drivingModule->stop();
  return true;
}

bool handleBumperEvent()
{
  if (bumperModule.hasCollided(5) != Sides::NONE)
  {
    drivingModule->stop();
  }
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