#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(Stream *stream,
                                 IDrivingModule *drivingModule,
                                 ISensorModule *sensorModule,
                                 SensorResult* sensorResult)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _sensorModule = sensorModule;
    _maxSensorResultAge = MAX_SENSOR_RESULT_AGE;
    _sensorResult = sensorResult;
}

bool AutoPilotModule::enabled()
{
    return _enabled;
}

void AutoPilotModule::enable()
{
    _enabled = true;
}

void AutoPilotModule::disable()
{
    _enabled = false;
}

void AutoPilotModule::handle()
{
    if (!enabled())
    {
        return;
    }

    _sensorModule->scan(_sensorResult);
    
    if (outOfDate())
    {
        _drivingModule->stop();
        return;
    }

    if (isTrapped())
    {
        _drivingModule->stop();
        return;
    }

    if (!spaceAhead())
    {
        if (_sensorResult->Right.Distance > _sensorResult->Left.Distance)
        {
            _drivingModule->turnRight();
            return;
        }
        if (_sensorResult->Left.Distance > _sensorResult->Right.Distance)
        {
            _drivingModule->turnLeft();
            return;
        }
        // if equal, bias for right
        _drivingModule->turnRight();
        return;
    }

    if (_sensorResult->Left.Distance < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _drivingModule->turnRight();
        return;
    }
    if (_sensorResult->Right.Distance < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _drivingModule->turnLeft();
        return;
    }

    if (_sensorResult->Left.Distance < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    if (_sensorResult->Right.Distance < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }

    if (isCentered())
    {
        _drivingModule->moveForward();
        return;
    }

    if (isOneSideClear())
    {
        _drivingModule->moveForward();
        return;
    }

    if (_sensorResult->Left.Distance < _sensorResult->Right.Distance)
    {
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }

    _drivingModule->bearLeft(DIR_FORWARD);
    return;
}

bool AutoPilotModule::outOfDate()
{
    unsigned long sensorResultAge = millis() - _sensorResult->Front.Timestamp;

    return (sensorResultAge > _maxSensorResultAge);
}

unsigned short AutoPilotModule::maxSensorResultAge()
{
    return _maxSensorResultAge;
}

void AutoPilotModule::updateResult(SensorResult *result)
{
    _sensorResult->Front.Distance = result->Front.Distance;
    _sensorResult->Front.Timestamp = result->Front.Timestamp;
    _sensorResult->Left.Distance = result->Left.Distance;
    _sensorResult->Left.Timestamp = result->Left.Timestamp;
    _sensorResult->Right.Distance = result->Right.Distance;
    _sensorResult->Right.Timestamp = result->Right.Timestamp;
}

bool AutoPilotModule::isCentered()
{
    if ((_sensorResult->Left.Distance - CENTER_TOLERANCE) < _sensorResult->Right.Distance && _sensorResult->Right.Distance < (_sensorResult->Left.Distance + CENTER_TOLERANCE))
    {
        return true;
    }
    if ((_sensorResult->Right.Distance - CENTER_TOLERANCE) < _sensorResult->Left.Distance && _sensorResult->Left.Distance < (_sensorResult->Right.Distance + CENTER_TOLERANCE))
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::isOneSideClear()
{
    if (_sensorResult->Left.Distance > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    if (_sensorResult->Right.Distance > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::spaceAhead()
{
    return _sensorResult->Front.Distance > FRONT_SENSOR_COLLISION_THRESHOLD;
}

bool AutoPilotModule::isTrapped()
{
    if (_sensorResult->Right.Distance > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    if (_sensorResult->Left.Distance > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    return true;
}