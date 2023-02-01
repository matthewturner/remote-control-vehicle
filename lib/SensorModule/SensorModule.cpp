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

const short POSITION_DELAY = 1000;
const short MESSAGE_DELAY = 300;

void SensorModule::handle(SensorResult *r)
{
    int desiredPosition = _stages[_desiredStage];
    int currentPosition = _servo.read();
    Serial.print("Desired: ");
    Serial.println(desiredPosition);
    Serial.print("Current: ");
    Serial.println(currentPosition);

    if (currentPosition == desiredPosition)
    {
        detect(r);
        _desiredStage++;
        if (_desiredStage > 3)
        {
            _desiredStage = 0;
        }
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