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

byte SensorModule::detect(SensorResult *r)
{
    r->Front = _sensorBuffer[0];
    r->Left = _sensorBuffer[1];
    r->Right = _sensorBuffer[2];
    r->Back = _sensorBuffer[3];
    r->Age = _sensorBuffer[4];

    return 6;
}