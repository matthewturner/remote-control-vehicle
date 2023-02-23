#ifndef BumperModule_h
#define BumperModule_h

#include <Arduino.h>
#include "IBumperModule.h"

class BumperModule : public IBumperModule
{
public:
    BumperModule(byte leftSensorPin, byte rightSensorPin,
                  Stream *stream);

    void begin() override;

    Sides hasCollided(unsigned short sinceMs = 0) override;

private:
    byte _leftSensorPin;
    byte _rightSensorPin;
    unsigned long _lastLeftCollision = 0;
    unsigned long _lastRightCollision = 0;
    Stream *_stream;
};

#endif