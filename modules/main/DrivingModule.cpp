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
    _isMoving = true;
    DBGP("Bearing left...");
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
    DBGP("Bearing left...");
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
    DBGP("Turning left...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight()
{
    _isMoving = true;
    DBGP("Turning right...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward()
{
    _isMoving = true;
    DBGP("Reversing...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward()
{
    _isMoving = true;
    DBGP("Moving forward...");
    byte actualSpeed = convertSpeed(_speed);
    // DBGP(actualSpeed);
    _motorLeft.forward(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    _isMoving = false;
    DBGP("Stopping...");
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