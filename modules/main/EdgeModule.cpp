#include <Arduino.h>
#include "EdgeModule.h"
#include "Debug.h"

EdgeModule::EdgeModule(HardwareSerial *stream)
{
    _stream = stream;
    reset();
}

void EdgeModule::increase()
{
    _duration += 100;
    if (_duration >= 500)
    {
        _duration = 500;
    }
}

void EdgeModule::decrease()
{
    _duration -= 100;
    if (_duration <= 100)
    {
        _duration = 100;
    }
}

void EdgeModule::reset()
{
    _duration = DEFAULT_DURATION;
}

short EdgeModule::duration()
{
    return _duration;
}