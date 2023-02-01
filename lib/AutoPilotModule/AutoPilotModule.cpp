#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(Stream *stream,
                                 IDrivingModule *drivingModule,
                                 ISensorModule *sensorModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _sensorModule = sensorModule;
    _resultAge = MAX_SENSOR_RESULT_AGE + 10;
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
            turnRight();
            return;
        }
        if (_sensorResult.Left > _sensorResult.Right)
        {
            turnLeft();
            return;
        }
        // if equal, bias for right
        turnRight();
        return;
    }

    if (_sensorResult.Left < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        turnRight();
        return;
    }
    if (_sensorResult.Right < SIDE_SENSOR_COLLISION_THRESHOLD)
    {
        turnLeft();
        return;
    }

    if (_sensorResult.Left < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        bearRight();
        return;
    }
    if (_sensorResult.Right < SIDE_SENSOR_COLLISION_WARNING_THRESHOLD)
    {
        bearLeft();
        return;
    }

    if (isCentered())
    {
        moveForward();
        return;
    }

    if (isOneSideClear())
    {
        moveForward();
        return;
    }

    if (_sensorResult.Left < _sensorResult.Right)
    {
        bearRight();
        return;
    }

    bearLeft();
    return;
}

void AutoPilotModule::moveForward()
{
    _maxSensorResultAge = _sensorResult.Front * MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD;
    _drivingModule->moveForward();
}

void AutoPilotModule::turnRight()
{
    _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
    _drivingModule->turnRight();
}

void AutoPilotModule::turnLeft()
{
    _maxSensorResultAge = MAX_SENSOR_AGE_FOR_TURN;
    _drivingModule->turnLeft();
}

void AutoPilotModule::bearRight()
{
    _maxSensorResultAge = _sensorResult.Left * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
    _drivingModule->bearRight(DIR_FORWARD);
}

void AutoPilotModule::bearLeft()
{
    _maxSensorResultAge = _sensorResult.Right * MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR;
    _drivingModule->bearLeft(DIR_FORWARD);
}

unsigned long AutoPilotModule::resultAge()
{
    return _resultAge;
}

unsigned int AutoPilotModule::maxSensorResultAge()
{
    return _maxSensorResultAge;
}

void AutoPilotModule::updateResult(SensorResult *result)
{
    _sensorResult.Age = result->Age;
    _sensorResult.Left = result->Left;
    _sensorResult.Right = result->Right;
    _sensorResult.Front = result->Front;
    _resultAge = millis();
}

SensorResult *AutoPilotModule::sensorResult()
{
    return &_sensorResult;
}

bool AutoPilotModule::updatePositionIfRequired()
{
    unsigned long sensorResultAge = millis() - _resultAge - _sensorResult.Age;

    if (_sensorModule->signalled() || (sensorResultAge > _maxSensorResultAge))
    {
        _drivingModule->stop();

        _sensorModule->detect(&_sensorResult);

        _resultAge = millis();

        debugPrintln("Sensor Module Result:");
        debugPrint("   ");
        debugPrintln(_sensorResult.Front);
        debugPrint(_sensorResult.Left);
        debugPrint("       ");
        debugPrintln(_sensorResult.Right);
        debugPrint("   ");
        debugPrintln(_sensorResult.Back);
        debugPrint("   ~");
        debugPrintln(_sensorResult.Age);

        return true;
    }

    return false;
}

bool AutoPilotModule::isCentered()
{
    if ((_sensorResult.Left - CENTER_TOLERANCE) < _sensorResult.Right && _sensorResult.Right < (_sensorResult.Left + CENTER_TOLERANCE))
    {
        return true;
    }
    if ((_sensorResult.Right - CENTER_TOLERANCE) < _sensorResult.Left && _sensorResult.Left < (_sensorResult.Right + CENTER_TOLERANCE))
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