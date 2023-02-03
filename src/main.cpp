#include "main.h"

void setup()
{
  Serial.begin(9600);

  bumperModule.begin();

  sensorModule->begin();

  pinMode(RECEIVE_PIN, INPUT);
  pinMode(TRANSMIT_PIN, OUTPUT);
  bluetoothSerial.begin(9600);

  commandListener.when("stp", (EvtCommandAction)stop);
  commandListener.when("fwd", (EvtCommandAction)forward);
  commandListener.when("rev", (EvtCommandAction)reverse);
  commandListener.when("lft", (EvtCommandAction)left);
  commandListener.when("rht", (EvtCommandAction)right);
  commandListener.when("b-lf", (EvtCommandAction)bearLeftForward);
  commandListener.when("b-rf", (EvtCommandAction)bearRightForward);
  commandListener.when("b-lr", (EvtCommandAction)bearLeftReverse);
  commandListener.when("b-rr", (EvtCommandAction)bearRightReverse);
  commandListener.when("spd", (EvtCommandAction)setSpeed);
  mgr.addListener(&commandListener);

  mgr.addListener(new EvtPinListener(LEFT_BUMPER_PIN, (EvtAction)handleBumperEvent));
  mgr.addListener(new EvtPinListener(RIGHT_BUMPER_PIN, (EvtAction)handleBumperEvent));

  Serial.println(F("Setup complete, continuing..."));
}

void loop()
{
  mgr.loopIteration();
}

bool stop()
{
  drivingModule->stop();
  return true;
}

bool detect()
{
  Direction Sequence[] = { Direction::LEFT, Direction::FRONT, Direction::RIGHT, Direction::FRONT };
  Direction direction = Sequence[currentDirection];

  if (sensorModule->request(&sensorResult, direction))
  {
    switch (direction)
    {
    case Direction::LEFT:
      Serial.print(F("LEFT  :"));
      Serial.println(sensorResult.Left.Distance);
      break;
    case Direction::RIGHT:
      Serial.print(F("RIGHT :"));
      Serial.println(sensorResult.Right.Distance);
      break;
    default:
      Serial.print(F("FRONT :"));
      Serial.println(sensorResult.Front.Distance);
      break;
    }
    currentDirection++;
    currentDirection %= 4;
  }
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