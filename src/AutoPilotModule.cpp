#include <Arduino.h>
#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(HardwareSerial *stream,
                                 IDrivingModule *drivingModule,
                                 CommandModule *commandModule,
                                 SensorModule *sensorModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _commandModule = commandModule;
    _sensorModule = sensorModule;
    _sampleAge = MAX_SENSOR_RESULT_AGE + 10;
    _maxSensorResultAge = MAX_SENSOR_RESULT_AGE;
}

void AutoPilotModule::handle()
{
    if (!_commandModule->selfDriveMode())
    {
        return;
    }

    SensorResult *sensorResult = &_sensorResult;

    unsigned long sensorResultAge = millis() - _sampleAge + sensorResult->Age;

    if (_sensorModule->signalled() || (sensorResultAge > _maxSensorResultAge))
    {
        _drivingModule->stop();

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

    if (isTrapped(sensorResult))
    {
        _drivingModule->stop();
        return;
    }

    if (!spaceAhead(sensorResult))
    {
        if (sensorResult->Right > sensorResult->Left)
        {
            _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
            _drivingModule->turnRight();
            return;
        }
        if (sensorResult->Left > sensorResult->Right)
        {
            _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
            _drivingModule->turnLeft();
            return;
        }
        // if equal, bias for right
        _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
        _drivingModule->turnRight();
        return;
    }

    if (sensorResult->Left < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
        _drivingModule->turnRight();
        return;
    }
    if (sensorResult->Right < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
        _drivingModule->turnLeft();
        return;
    }
    if (sensorResult->Left < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _maxSensorResultAge = sensorResult->Left * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    if (sensorResult->Right < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _maxSensorResultAge = sensorResult->Right * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
    if (isCentered(sensorResult))
    {
        _maxSensorResultAge = sensorResult->Front * MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD;
        _drivingModule->moveForward();
        return;
    }
    if (isOneSideClear(sensorResult))
    {
        _maxSensorResultAge = sensorResult->Front * MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD;
        _drivingModule->moveForward();
        return;
    }
    if (sensorResult->Left < sensorResult->Right)
    {
        _maxSensorResultAge = sensorResult->Left * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    else
    {
        _maxSensorResultAge = sensorResult->Right * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
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