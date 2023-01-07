#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(Stream *stream,
                                 IDrivingModule *drivingModule,
                                 ISensorModule *sensorModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _sensorModule = sensorModule;
    _sampleAge = MAX_SENSOR_RESULT_AGE + 10;
    _maxSensorResultAge = MAX_SENSOR_RESULT_AGE;
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

    updatePositionIfRequired();

    if (isTrapped())
    {
        _drivingModule->stop();
        return;
    }

    if (!spaceAhead())
    {
        if (_sensorResult.Right > _sensorResult.Left)
        {
            _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
            _drivingModule->turnRight();
            return;
        }
        if (_sensorResult.Left > _sensorResult.Right)
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

    if (_sensorResult.Left < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
        _drivingModule->turnRight();
        return;
    }
    if (_sensorResult.Right < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
        _drivingModule->turnLeft();
        return;
    }
    if (_sensorResult.Left < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _maxSensorResultAge = _sensorResult.Left * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    if (_sensorResult.Right < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        _maxSensorResultAge = _sensorResult.Right * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
    if (isCentered())
    {
        _maxSensorResultAge = _sensorResult.Front * MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD;
        _drivingModule->moveForward();
        return;
    }
    if (isOneSideClear())
    {
        _maxSensorResultAge = _sensorResult.Front * MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD;
        _drivingModule->moveForward();
        return;
    }
    if (_sensorResult.Left < _sensorResult.Right)
    {
        _maxSensorResultAge = _sensorResult.Left * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearRight(DIR_FORWARD);
        return;
    }
    else
    {
        _maxSensorResultAge = _sensorResult.Right * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
        _drivingModule->bearLeft(DIR_FORWARD);
        return;
    }
}

unsigned long AutoPilotModule::sampleAge()
{
    return _sampleAge;
}

unsigned int AutoPilotModule::maxSensorResultAge()
{
    return _maxSensorResultAge;
}

bool AutoPilotModule::updatePositionIfRequired()
{
    unsigned long sensorResultAge = millis() - _sampleAge + _sensorResult.Age;

    if (_sensorModule->signalled() || (sensorResultAge > _maxSensorResultAge))
    {
        _drivingModule->stop();

        _sensorModule->detect(&_sensorResult);

        _sampleAge = millis();

        debugPrintln("Sensor Module Result:");
        debugPrint("   ");
        debugPrintln(sensorResult.Front);
        debugPrint(sensorResult.Left);
        debugPrint("       ");
        debugPrintln(sensorResult.Right);
        debugPrint("   ");
        debugPrintln(sensorResult.Back);
        debugPrint("   ~");
        debugPrintln(sensorResult.Age);
    }
}

bool AutoPilotModule::isCentered()
{
    if (_sensorResult.Left - CENTER_TOLERANCE < _sensorResult.Right && _sensorResult.Right < _sensorResult.Left + CENTER_TOLERANCE)
    {
        return true;
    }
    if (_sensorResult.Right - CENTER_TOLERANCE < _sensorResult.Left && _sensorResult.Left < _sensorResult.Right + CENTER_TOLERANCE)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::isOneSideClear()
{
    if (_sensorResult.Left > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    if (_sensorResult.Right > SIDE_SENSOR_CLEAR_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool AutoPilotModule::spaceAhead()
{
    return _sensorResult.Front > FRONT_SENSOR_COLLISION_THRESHOLD;
}

bool AutoPilotModule::isTrapped()
{
    if (_sensorResult.Right > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    if (_sensorResult.Left > SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        return false;
    }
    return true;
}