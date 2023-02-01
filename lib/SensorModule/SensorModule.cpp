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

void SensorModule::scan(SensorResult *r)
{
    unsigned long now = millis();
    unsigned long elapsed = now - _lastChange;

    if (elapsed > POSITION_DELAY)
    {
        detect(r);
        _desiredPosition++;
        _desiredPosition %= 4;
        int desiredPosition = _positions[_desiredPosition];
        _servo.write(desiredPosition);
        _lastChange = now;
    }
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
    switch(_desiredPosition)
    {
        case 0:
            r->Left = _measure.RangeMilliMeter;
            break;
        case 2:
            r->Right = _measure.RangeMilliMeter;
            break;
        default:
            r->Front = _measure.RangeMilliMeter;
            break;
    }

    return true;
}