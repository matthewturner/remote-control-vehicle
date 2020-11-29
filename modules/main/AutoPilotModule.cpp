#include <Arduino.h>
#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(HardwareSerial *stream,
                                 DrivingModule *drivingModule,
                                 CommandModule *commandModule,
                                 SensorModule *sensorModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _commandModule = commandModule;
    _sensorModule = sensorModule;
    _sampleAge = MAX_SENSOR_RESULT_AGE + 10;
}

void AutoPilotModule::handle()
{
    SensorResult *sensorResult = &_sensorResult;

    if ((_sampleAge + sensorResult->Age > MAX_SENSOR_RESULT_AGE) || _sensorModule->signalled())
    {
        _sensorModule->detect(sensorResult);

        _sampleAge = millis();

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
    }

    if (!_commandModule->selfDriveMode())
    {
        if (!spaceAhead(sensorResult))
        {
            if (_drivingModule->directionOfMotion() == MOVE_FORWARD)
            {
                _drivingModule->stop();
            }
        }
        return;
    }

    if (isTrapped(sensorResult))
    {
        _drivingModule->stop();
        return;
    }

    if (!spaceAhead(sensorResult))
    {
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
        // if equal, bias for right
        _drivingModule->turnRight();
        return;
    }

    if (sensorResult->Left < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _drivingModule->turnRight();
        return;
    }
    if (sensorResult->Right < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _drivingModule->turnLeft();
        return;
    }
    if (sensorResult->Left < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    if (sensorResult->Right < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
    if (isCentered(sensorResult))
    {
        _drivingModule->moveForward();
        return;
    }
    if (isOneSideClear(sensorResult))
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
    if (sensorResult->Left - CENTER_TOLERANCE < sensorResult->Right < sensorResult->Left + CENTER_TOLERANCE)
    {
        return true;
    }
    if (sensorResult->Right - CENTER_TOLERANCE < sensorResult->Left < sensorResult->Right + CENTER_TOLERANCE)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::isOneSideClear(SensorResult *sensorResult)
{
    if (sensorResult->Left > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    if (sensorResult->Right > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::spaceAhead(SensorResult *sensorResult)
{
    return sensorResult->Front > FRONT_SENSOR_COLLISION_THRESHOLD;
}

bool AutoPilotModule::isTrapped(SensorResult *sensorResult)
{
    if (sensorResult->Right > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    if (sensorResult->Left > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    return true;
}