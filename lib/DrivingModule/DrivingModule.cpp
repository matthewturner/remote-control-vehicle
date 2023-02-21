#include "DrivingModule.h"
#include "Debug.h"

DrivingModule::DrivingModule(byte motorLeftEnablePin, byte motorLeftDirectionPin,
                             byte motorRightEnablePin, byte motorRightDirectionPin,
                             Stream *stream)
    : _motorLeft(motorLeftEnablePin, motorLeftDirectionPin),
      _motorRight(motorRightEnablePin, motorRightDirectionPin)
{
    _speed = DEFAULT_SPEED;
    _stream = stream;
}

void DrivingModule::bearLeft(bool forward)
{
    if (forward)
    {
        if (_directionOfMotion != Motion::BEAR_LEFT_FORWARD)
        {
            return;
        }
    }
    else
    {
        if (_directionOfMotion != Motion::BEAR_LEFT_REVERSE)
        {
            return;
        }
    }

    debugPrintln(F("Bearing left..."));
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > BEAR_SPEED_THRESHOLD)
    {
        reducedSpeed = convertSpeed(_speed - BEAR_SPEED_THRESHOLD);
    }
    if (forward)
    {
        _directionOfMotion = Motion::FORWARD;
        _motorLeft.forward(reducedSpeed);
        _motorRight.forward(actualSpeed);
    }
    else
    {
        _directionOfMotion = Motion::REVERSE;
        _motorLeft.back(reducedSpeed);
        _motorRight.back(actualSpeed);
    }
}

void DrivingModule::bearRight(bool forward)
{
    if (forward)
    {
        if (_directionOfMotion != Motion::BEAR_RIGHT_FORWARD)
        {
            return;
        }
    }
    else
    {
        if (_directionOfMotion != Motion::BEAR_RIGHT_REVERSE)
        {
            return;
        }
    }

    debugPrintln(F("Bearing right..."));
    byte actualSpeed = convertSpeed(_speed);
    byte reducedSpeed = 0;
    if (_speed > BEAR_SPEED_THRESHOLD)
    {
        reducedSpeed = convertSpeed(_speed - BEAR_SPEED_THRESHOLD);
    }
    if (forward)
    {
        _directionOfMotion = Motion::FORWARD;
        _motorLeft.forward(actualSpeed);
        _motorRight.forward(reducedSpeed);
    }
    else
    {
        _directionOfMotion = Motion::REVERSE;
        _motorLeft.back(actualSpeed);
        _motorRight.back(reducedSpeed);
    }
}

void DrivingModule::turnLeft()
{
    if (_directionOfMotion == Motion::LEFT)
    {
        return;
    }

    _directionOfMotion = Motion::LEFT;
    debugPrintln(F("Turning left..."));
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::turnRight()
{
    if (_directionOfMotion == Motion::RIGHT)
    {
        return;
    }

    _directionOfMotion = Motion::RIGHT;
    debugPrintln(F("Turning right..."));
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.forward(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveBackward()
{
    if (_directionOfMotion == Motion::REVERSE)
    {
        return;
    }

    _directionOfMotion = Motion::REVERSE;
    debugPrintln(F("Reversing..."));
    byte actualSpeed = convertSpeed(_speed);
    _motorLeft.back(actualSpeed);
    _motorRight.back(actualSpeed);
}

void DrivingModule::moveForward()
{
    if (_directionOfMotion == Motion::FORWARD)
    {
        return;
    }

    _directionOfMotion = Motion::FORWARD;
    debugPrintln(F("Moving forward..."));
    byte actualSpeed = convertSpeed(_speed);
    // debugPrintln(actualSpeed);
    _motorLeft.forward(actualSpeed);
    _motorRight.forward(actualSpeed);
}

void DrivingModule::stop()
{
    if (_directionOfMotion == Motion::STOPPED)
    {
        return;
    }

    _directionOfMotion = Motion::STOPPED;
    debugPrintln(F("Stopping..."));
    _motorLeft.stop();
    _motorRight.stop();
}

bool DrivingModule::isMoving()
{
    return (_directionOfMotion != Motion::STOPPED);
}

Motion DrivingModule::directionOfMotion()
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
    debugPrint(F("Setting speed: "));
    debugPrintln(speed);
    _speed = speed;
}

void DrivingModule::resetSpeed()
{
    _speed = DEFAULT_SPEED;
}