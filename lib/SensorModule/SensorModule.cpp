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
        _desiredStage++;
        if (_desiredStage > 3)
        {
            _desiredStage = 0;
        }
        int desiredPosition = _stages[_desiredStage];
        _servo.write(desiredPosition);
        _lastChange = now;
    }
}

byte SensorModule::detect(SensorResult *r)
{
    _sensor.rangingTest(&_measure, false);

    if (_measure.RangeStatus != 4)
    {
        Serial.println(_measure.RangeMilliMeter);
        r->Right = _measure.RangeMilliMeter;
    }
    else
    {
        Serial.println(F(" out of range "));
    }

    return 6;
}