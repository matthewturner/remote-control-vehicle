#ifndef BumperModule_h
#define BumperModule_h

#include <Arduino.h>

typedef enum : byte
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTH = LEFT | RIGHT
} Sides;

class BumperModule
{
public:
    BumperModule(byte leftSensorPin, byte rightSensorPin,
                  Stream *stream);

    Sides hasCollided(short sinceMs = 0);

private:
    byte _leftSensorPin;
    byte _rightSensorPin;
    unsigned long _lastLeftCollision = 0;
    unsigned long _lastRightCollision = 0;
    Stream *_stream;
};

#endif