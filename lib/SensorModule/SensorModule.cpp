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
}

bool SensorModule::signalled()
{
    return false;
}

const short POSITION_DELAY = 200;
const short MESSAGE_DELAY = 300;

byte SensorModule::detect(SensorResult *r)
{
    Serial.println(F("Facing right (0)"));
    Serial.print(F("Right Distance in mm: "));
    _servo.write(0);
    delay(POSITION_DELAY);
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
    delay(MESSAGE_DELAY);

    Serial.println(F("Facing forward (90)"));
    Serial.print(F("Front Distance in mm: "));
    _servo.write(90);
    delay(POSITION_DELAY);
    _sensor.rangingTest(&_measure, false);

    if (_measure.RangeStatus != 4)
    {
        Serial.println(_measure.RangeMilliMeter);
        r->Front = _measure.RangeMilliMeter;
    }
    else
    {
        Serial.println(F(" out of range "));
    }
    delay(MESSAGE_DELAY);

    Serial.println(F("Facing left (180)"));
    Serial.print(F("Left Distance in mm: "));
    _servo.write(180);
    delay(POSITION_DELAY);
    _sensor.rangingTest(&_measure, false);

    if (_measure.RangeStatus != 4)
    {
        Serial.println(_measure.RangeMilliMeter);
        r->Left = _measure.RangeMilliMeter;
    }
    else
    {
        Serial.println(F(" out of range "));
    }
    delay(MESSAGE_DELAY);

    Serial.println(F("Facing forward (90)"));
    Serial.print(F("Front Distance in mm: "));
    _servo.write(90);
    delay(POSITION_DELAY);
    _sensor.rangingTest(&_measure, false);

    if (_measure.RangeStatus != 4)
    {
        Serial.println(_measure.RangeMilliMeter);
        r->Front = _measure.RangeMilliMeter;
    }
    else
    {
        Serial.println(F(" out of range "));
    }
    delay(MESSAGE_DELAY);

    return 6;
}