#ifndef BumperModule_h
#define BumperModule_h

#include <Arduino.h>

enum class Sides : byte
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTH = LEFT | RIGHT
};

class BumperModule
{
public:
    BumperModule(byte leftSensorPin, byte rightSensorPin,
                  Stream *stream);

    void begin();

    Sides hasCollided(unsigned short sinceMs = 0);

private:
    byte _leftSensorPin;
    byte _rightSensorPin;
    unsigned long _lastLeftCollision = 0;
    unsigned long _lastRightCollision = 0;
    Stream *_stream;
};

#endif