#include "SensorModule.h"

SensorModule::SensorModule(byte i2cAddress, byte servoControlPin,
                           Stream *stream)
{
    _i2cAddress = i2cAddress;
    _servoControlPin = servoControlPin;
    _stream = stream;
}

void SensorModule::begin()
{
    _servo.attach(_servoControlPin);
    _sensor.begin(_i2cAddress);
    _servo.write(0);
}

bool SensorModule::signalled()
{
    return false;
}

bool SensorModule::request(SensorResult *r, Direction direction)
{
    unsigned long now = millis();

    if (direction != _desiredDirection)
    {
        _desiredDirection = direction;        
        byte desiredPosition = _positions[(byte)_desiredDirection];
        _servo.write(desiredPosition);
        _lastChange = now;
        return false;
    }

    unsigned long elapsed = now - _lastChange;

    if (elapsed <= POSITION_DELAY)
    {
        return false;
    }

    return detect(r);
}

bool SensorModule::detect(SensorResult *r)
{
    _sensor.rangingTest(&_measure, false);

    if (_measure.RangeStatus == 4)
    {
        // out of range
        return false;
    }

    Serial.println(_measure.RangeMilliMeter);
    switch(_desiredDirection)
    {
        case Direction::LEFT:
            r->Left.Distance = _measure.RangeMilliMeter;
            r->Left.Timestamp = millis();
            break;
        case Direction::RIGHT:
            r->Right.Distance = _measure.RangeMilliMeter;
            r->Right.Timestamp = millis();
            break;
        default:
            r->Front.Distance = _measure.RangeMilliMeter;
            r->Front.Timestamp = millis();
            break;
    }

    return true;
}