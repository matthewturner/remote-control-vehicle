#ifndef IDrivingModule_h
#define IDrivingModule_h

#include <Arduino.h>

const bool DIR_FORWARD = true;
const bool DIR_REVERSE = false;

typedef enum : byte
{
    STOPPED = 0,
    FORWARD = 1,
    LEFT = 2,
    RIGHT = 3,
    BACKWARD = 4
} Motion;

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

    virtual byte directionOfMotion() = 0;

    virtual void setSpeed(byte speed) = 0;

    virtual void resetSpeed() = 0;
};

#endif