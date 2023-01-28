#include "BumperModule.h"

BumperModule::BumperModule(byte leftSensorPin, byte rightSensorPin, Stream *stream)
{
    _leftSensorPin = leftSensorPin;
    _rightSensorPin = rightSensorPin;
    _stream = stream;
}

void BumperModule::begin()
{
    pinMode(_leftSensorPin, INPUT_PULLUP);
    pinMode(_rightSensorPin, INPUT_PULLUP);
}

Sides BumperModule::hasCollided(unsigned short sinceMs)
{
    Sides sides = Sides::NONE;
    unsigned long time = millis();

    if (digitalRead(_leftSensorPin) == LOW)
    {
        sides = (Sides)((byte)sides | (byte)Sides::LEFT);
        _lastLeftCollision = time;
    }
    else
    {
        if ((time - _lastLeftCollision) <= sinceMs)
        {
            sides = (Sides)((byte)sides | (byte)Sides::LEFT);  
        }
    }

    if (digitalRead(_rightSensorPin) == LOW)
    {
        sides = (Sides)((byte)sides | (byte)Sides::RIGHT);
        _lastRightCollision = time;
    }
    else
    {
        if ((time - _lastRightCollision) <= sinceMs)
        {
            sides = (Sides)((byte)sides | (byte)Sides::RIGHT);  
        }
    }

    return sides;
}