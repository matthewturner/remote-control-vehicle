#include "BumperModule.h"

BumperModule::BumperModule(byte leftSensorPin, byte rightSensorPin, Stream *stream)
{
    _leftSensorPin = leftSensorPin;
    _rightSensorPin = rightSensorPin;
    _stream = stream;
}

Sides BumperModule::hasCollided(short sinceMs)
{
    Sides sides = Sides::NONE;
    unsigned long time = millis();

    if (digitalRead(_leftSensorPin) == LOW)
    {
        sides = (Sides)(sides | Sides::LEFT);
        _lastLeftCollision = time;
    }
    else
    {
        if ((time - _lastLeftCollision) <= sinceMs)
        {
            sides = (Sides)(sides | Sides::LEFT);  
        }
    }

    if (digitalRead(_rightSensorPin) == LOW)
    {
        sides = (Sides)(sides | Sides::RIGHT);
        _lastRightCollision = time;
    }
    else
    {
        if ((time - _lastRightCollision) <= sinceMs)
        {
            sides = (Sides)(sides | Sides::RIGHT);  
        }
    }

    return sides;
}