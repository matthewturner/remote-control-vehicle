#include <Wire.h>
#include <Arduino.h>
#include "SensorModule.h"

SensorModule::SensorModule(byte i2cAddress)
{
    _i2cAddress = i2cAddress;
}

void SensorModule::begin()
{
    Wire.begin();
}

SensorResult SensorModule::detect()
{
    Wire.requestFrom(_i2cAddress, 6);

    byte counter = 0;
    while (Wire.available())
    {
        char c = Wire.read();
        _sensorBuffer[counter] = c;
        counter++;
    }

    SensorResult r;
    r.Forward = _sensorBuffer[0];
    r.Left = _sensorBuffer[1];
    r.Right = _sensorBuffer[2];
    r.Back = _sensorBuffer[3];
    r.Age = _sensorBuffer[4];

    return r;
}