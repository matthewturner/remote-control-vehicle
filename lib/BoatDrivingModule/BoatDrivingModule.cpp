#include "BoatDrivingModule.h"
#include "Debug.h"

BoatDrivingModule::BoatDrivingModule(IDrivingModule *drivingModule, RudderModule *rudderModule)
{
    _drivingModule = drivingModule;
    _rudderModule = rudderModule;
}

void BoatDrivingModule::bearLeft(bool forward)
{
    _rudderModule->port();
    if (forward)
    {
        _drivingModule->moveForward();
    }
    else
    {
        _drivingModule->moveBackward();
    }
}

void BoatDrivingModule::bearRight(bool forward)
{
    _rudderModule->starboard();
    if (forward)
    {
        _drivingModule->moveForward();
    }
    else
    {
        _drivingModule->moveBackward();
    }
}

void BoatDrivingModule::turnLeft()
{
    _drivingModule->turnLeft();
}

void BoatDrivingModule::turnRight()
{
    _drivingModule->turnRight();
}

void BoatDrivingModule::moveBackward()
{
    _rudderModule->ahead();
    _drivingModule->moveBackward();
}

void BoatDrivingModule::moveForward()
{
    _rudderModule->ahead();
    _drivingModule->moveBackward();
}

void BoatDrivingModule::stop()
{
    _drivingModule->stop();
}

bool BoatDrivingModule::isMoving()
{
    return _drivingModule->isMoving();
}

Motion BoatDrivingModule::directionOfMotion()
{
    return _drivingModule->directionOfMotion();
}

byte BoatDrivingModule::convertSpeed(byte speed)
{
    return _drivingModule->convertSpeed(speed);
}

void BoatDrivingModule::setSpeed(byte speed)
{
    return _drivingModule->setSpeed(speed);
}

void BoatDrivingModule::resetSpeed()
{
    _drivingModule->resetSpeed();
}