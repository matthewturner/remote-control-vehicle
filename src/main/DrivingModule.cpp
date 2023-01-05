#include <Arduino.h>
#include <L293.h>
#include "DrivingModule.h"
#include "Debug.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                             byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin,
                             HardwareSerial *stream)
    : _motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin),
      _motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin)
{
    _speed = DEFAULT_SPEED;
    _stream = stream;
}

void DrivingModule::bearLeft(bool forward)
{
    DBGP("Bearing left...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > 4)
    {
        reducedSpeed = convertSpeed(_speed - 4);
    }
    if (forward)
    {
        _directionOfMotion = MOVE_FORWARD;
        _motorLeft.forward(reducedSpeed);
        _motorRight.forward(actualSpeed);
    }
    else
    {
        _directionOfMotion = MOVE_BACKWARD;
        _motorLeft.back(reducedSpeed);
        _motorRight.back(actualSpeed);
    }
}

void DrivingModule::bearRight(bool forward)
{
    DBGP("Bearing right...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > 4)
    {
        reducedSpeed = convertSpeed(_speed - 4);
    }
    if (forward)
    {
        _directionOfMotion = MOVE_FORWARD;
        _motorLeft.forward(actualSpeed);
        _motorRight.forward(reducedSpeed);
    }
    else
    {
        _directionOfMotion = MOVE_BACKWARD;
        _motorLeft.back(actualSpeed);
        _motorRight.back(reducedSpeed);
    }
}

void DrivingModule::turnLeft()
{
    _directionOfMotion = MOVE_LEFT;
    DBGP("Turning left...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight()
{
    _directionOfMotion = MOVE_RIGHT;
    DBGP("Turning right...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward()
{
    _directionOfMotion = MOVE_BACKWARD;
    DBGP("Reversing...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward()
{
    _directionOfMotion = MOVE_FORWARD;
    DBGP("Moving forward...");
    byte actualSpeed = convertSpeed(_speed);
    // DBGP(actualSpeed);
    _motorLeft.forward(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    _directionOfMotion = MOVE_STOPPED;
    DBGP("Stopping...");
    _motorLeft.stop();
    _motorRight.stop();
}

bool DrivingModule::isMoving()
{
    return _directionOfMotion > 0;
}

byte DrivingModule::directionOfMotion()
{
    return _directionOfMotion;
}

byte DrivingModule::convertSpeed(byte speed)
{
    if (speed == 0)
    {
        return 0;
    }
    return map(speed, 1, 5, 50, 255);
}

void DrivingModule::setSpeed(byte speed)
{
    _speed = speed;
}

void DrivingModule::resetSpeed()
{
    _speed = DEFAULT_SPEED;
}