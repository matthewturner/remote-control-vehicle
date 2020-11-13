#include <Arduino.h>
#include <L293.h>
#include "DrivingModule.h"
#include "Debug.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                             byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin)
    : _motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin),
      _motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin)
{
}

void DrivingModule::bearLeft(byte speed, bool forward)
{
    _isMoving = true;
    DBG("Bearing left...");
    byte actualSpeed = convertSpeed(speed);
    byte reducedSpeed = 0;
    if (speed > 4)
    {
        reducedSpeed = convertSpeed(speed - 4);
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

void DrivingModule::bearRight(byte speed, bool forward)
{
    _isMoving = true;
    DBG("Bearing left...");
    byte actualSpeed = convertSpeed(speed);
    byte reducedSpeed = 0;
    if (speed > 4)
    {
        reducedSpeed = convertSpeed(speed - 4);
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

void DrivingModule::turnLeft(byte speed)
{
    _isMoving = true;
    DBG("Turning left...");
    byte actualSpeed = convertSpeed(speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight(byte speed)
{
    _isMoving = true;
    DBG("Turning right...");
    byte actualSpeed = convertSpeed(speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward(byte speed)
{
    _isMoving = true;
    DBG("Reversing...");
    byte actualSpeed = convertSpeed(speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward(byte speed)
{
    _isMoving = true;
    DBG("Moving forward...");
    byte actualSpeed = convertSpeed(speed);
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