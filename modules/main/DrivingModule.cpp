#include <Arduino.h>
#include <L293.h>
#include "DrivingModule.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                             byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin)
{
    _motorLeft = L293(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin);
    _motorRight = L293(motorRightEnablePin, motorRightForwardPin, motorRightReversePin);
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
        motorLeft.forward(reducedSpeed);
        motorRight.forward(actualSpeed);
    }
    else
    {
        motorLeft.back(reducedSpeed);
        motorRight.back(actualSpeed);
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
        motorLeft.forward(actualSpeed);
        motorRight.forward(reducedSpeed);
    }
    else
    {
        motorLeft.back(actualSpeed);
        motorRight.back(reducedSpeed);
    }
}

void DrivingModule::turnLeft(byte speed)
{
    _isMoving = true;
    DBG("Turning left...");
    byte actualSpeed = convertSpeed(speed);
    motorLeft.back(actualSpeed);
    motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight(byte speed)
{
    _isMoving = true;
    DBG("Turning right...");
    byte actualSpeed = convertSpeed(speed);
    motorLeft.forward(actualSpeed);
    motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward(byte speed)
{
    _isMoving = true;
    DBG("Reversing...");
    byte actualSpeed = convertSpeed(speed);
    motorLeft.back(actualSpeed);
    motorRight.back(actualSpeed);
}

void DrivingModule::moveForward(byte speed)
{
    _isMoving = true;
    DBG("Moving forward...");
    byte actualSpeed = convertSpeed(speed);
    // DBG(actualSpeed);
    motorLeft.forward(actualSpeed);
    motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    _isMoving = false;
    DBG("Stopping...");
    motorLeft.stop();
    motorRight.stop();
}

bool isMoving()
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