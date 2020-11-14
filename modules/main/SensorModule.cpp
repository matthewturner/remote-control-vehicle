#include <Wire.h>
#include <Arduino.h>
#include "SensorModule.h"

SensorModule::SensorModule(byte i2cAddress, HardwareSerial *stream)
{
    _i2cAddress = i2cAddress;
    _stream = stream;
}

void SensorModule::begin()
{
    Wire.begin();
}

byte SensorModule::detect(SensorResult *r)
{
    Wire.requestFrom(_i2cAddress, 6);

    byte counter = 0;
    while (Wire.available())
    {
        byte c = Wire.read();
        _sensorBuffer[counter] = c;
        counter++;
    }

    if (counter < 6)
    {
        return counter;
    }

    r->Front = _sensorBuffer[0];
    r->Left = _sensorBuffer[1];
    r->Right = _sensorBuffer[2];
    r->Back = _sensorBuffer[3];
    r->Age = _sensorBuffer[4];

    return counter;
}