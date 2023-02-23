#ifndef IDrivingModule_h
#define IDrivingModule_h

#include <Arduino.h>

const bool DIR_FORWARD = true;
const bool DIR_REVERSE = false;

enum class Motion : byte
{
    STOPPED = 0,
    FORWARD = 1,
    LEFT = 2,
    RIGHT = 4,
    REVERSE = 8,
    BEAR_LEFT_FORWARD = LEFT | FORWARD,
    BEAR_LEFT_REVERSE = LEFT | REVERSE,
    BEAR_RIGHT_FORWARD = RIGHT | FORWARD,
    BEAR_RIGHT_REVERSE = RIGHT | REVERSE
};

const byte DEFAULT_SPEED = 5;

class IDrivingModule
{
public:
    IDrivingModule();

    virtual void bearLeft(bool forward) = 0;

    virtual void bearRight(bool forward) = 0;

    virtual void turnLeft() = 0;

    virtual void turnRight() = 0;

    virtual void moveBackward() = 0;

    virtual void moveForward() = 0;

    virtual void stop() = 0;

    virtual byte convertSpeed(byte speed) = 0;

    virtual bool isMoving() = 0;

    virtual Motion directionOfMotion() = 0;

    virtual void setSpeed(byte speed) = 0;

    virtual void resetSpeed() = 0;
};

#endif