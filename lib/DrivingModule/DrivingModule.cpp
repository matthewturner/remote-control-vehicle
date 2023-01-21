#include "DrivingModule.h"
#include "Debug.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                             byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin,
                             Stream *stream)
    : _motorLeft(motorLeftEnablePin, motorLeftForwardPin, motorLeftReversePin),
      _motorRight(motorRightEnablePin, motorRightForwardPin, motorRightReversePin)
{
    _speed = DEFAULT_SPEED;
    _stream = stream;
}

void DrivingModule::bearLeft(bool forward)
{
    debugPrintln("Bearing left...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > BEAR_SPEED_THRESHOLD)
    {
        reducedSpeed = convertSpeed(_speed - BEAR_SPEED_THRESHOLD);
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
    debugPrintln("Bearing right...");
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > BEAR_SPEED_THRESHOLD)
    {
        reducedSpeed = convertSpeed(_speed - BEAR_SPEED_THRESHOLD);
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
    debugPrintln("Turning left...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight()
{
    _directionOfMotion = MOVE_RIGHT;
    debugPrintln("Turning right...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward()
{
    _directionOfMotion = MOVE_BACKWARD;
    debugPrintln("Reversing...");
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward()
{
    _directionOfMotion = MOVE_FORWARD;
    debugPrintln("Moving forward...");
    byte actualSpeed = convertSpeed(_speed);
    // debugPrintln(actualSpeed);
    _motorLeft.forward(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    _directionOfMotion = MOVE_STOPPED;
    debugPrintln("Stopping...");
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
    return map(speed, 1, 5, 100, 250);
}

void DrivingModule::setSpeed(byte speed)
{
    debugPrint("Setting speed: ");
    debugPrintln(speed);
    _speed = speed;
}

void DrivingModule::resetSpeed()
{
    _speed = DEFAULT_SPEED;
}