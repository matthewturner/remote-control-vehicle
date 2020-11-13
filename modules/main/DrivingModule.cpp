#include <Arduino.h>
#include <L293.h>
#include "DrivingModule.h"
#include "Debug.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                             byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin)
    : _motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin),
      _motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin)
{
    _speed = DEFAULT_SPEED;
}

void DrivingModule::bearLeft(bool forward)
{
    _isMoving = true;
    DBG("Bearing left...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > 4)
    {
        reducedSpeed = convertSpeed(_speed - 4);
    }
    if (forward)
    {
        _motorLeft.forward(reducedSpeed);
        _motorRight.forward(actualSpeed);
    }
    else
    {
        _motorLeft.back(reducedSpeed);
        _motorRight.back(actualSpeed);
    }
}

void DrivingModule::bearRight(bool forward)
{
    _isMoving = true;
    DBG("Bearing left...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > 4)
    {
        reducedSpeed = convertSpeed(_speed - 4);
    }
    if (forward)
    {
        _motorLeft.forward(actualSpeed);
        _motorRight.forward(reducedSpeed);
    }
    else
    {
        _motorLeft.back(actualSpeed);
        _motorRight.back(reducedSpeed);
    }
}

void DrivingModule::turnLeft()
{
    _isMoving = true;
    DBG("Turning left...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight()
{
    _isMoving = true;
    DBG("Turning right...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward()
{
    _isMoving = true;
    DBG("Reversing...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward()
{
    _isMoving = true;
    DBG("Moving forward...");
    byte actualSpeed = convertSpeed(_speed);
    // DBG(actualSpeed);
    _motorLeft.forward(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    _isMoving = false;
    DBG("Stopping...");
    _motorLeft.stop();
    _motorRight.stop();
}

bool DrivingModule::isMoving()
{
    return _isMoving;
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