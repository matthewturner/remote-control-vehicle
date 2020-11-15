#include <Arduino.h>
#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(HardwareSerial *stream,
                                 DrivingModule *drivingModule,
                                 CommandModule *commandModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _commandModule = commandModule;
}

void AutoPilotModule::handle(SensorResult *sensorResult)
{
    DBGP("Sensor Module Result:");
    DBGP_PRNT("   ");
    DBGP(sensorResult->Front);
    DBGP_PRNT(sensorResult->Left);
    DBGP_PRNT("       ");
    DBGP(sensorResult->Right);
    DBGP_PRNT("   ");
    DBGP(sensorResult->Back);
    DBGP_PRNT("   ~");
    DBGP(sensorResult->Age);

    if (!spaceAhead(sensorResult))
    {
        if (!_commandModule->selfDriveMode())
        {
            if (_drivingModule->directionOfMotion() == MOVE_FORWARD)
            {
                _drivingModule->stop();
            }
            return;
        }
        if (isTrapped(sensorResult))
        {
            _drivingModule->stop();
            return;
        }
        if (sensorResult->Right > sensorResult->Left)
        {
            _drivingModule->turnRight();
            return;
        }
        if (sensorResult->Left > sensorResult->Right)
        {
            _drivingModule->turnLeft();
            return;
        }
        // bias for right
        _drivingModule->turnRight();
        return;
    }

    if (sensorResult->Left < 4)
    {
        _drivingModule->turnRight();
        return;
    }
    if (sensorResult->Right < 4)
    {
        _drivingModule->turnLeft();
        return;
    }
    if (sensorResult->Left < 7)
    {
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    if (sensorResult->Right < 7)
    {
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
    if (isCentered(sensorResult))
    {
        _drivingModule->moveForward();
        return;
    }
    if (sensorResult->Left > 20 || sensorResult->Right > 20)
    {
        _drivingModule->moveForward();
        return;
    }
    if (sensorResult->Left < sensorResult->Right)
    {
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    else
    {
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
}

bool AutoPilotModule::isCentered(SensorResult *sensorResult)
{
    if (sensorResult->Left - 2 < sensorResult->Right < sensorResult->Left + 2)
    {
        return true;
    }
    if (sensorResult->Right - 2 < sensorResult->Left < sensorResult->Right + 2)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::spaceAhead(SensorResult *sensorResult)
{
    return sensorResult->Front > 7;
}

bool AutoPilotModule::isTrapped(SensorResult *sensorResult)
{
    if (spaceAhead(sensorResult))
    {
        return false;
    }
    if (sensorResult->Right > 4 || sensorResult->Left > 4)
    {
        return false;
    }
    return true;
}