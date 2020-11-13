#ifndef DrivingModule_h
#define DrivingModule_h

#include <L293.h>

#define DIR_FORWARD true
#define DIR_REVERSE false

#define DEFAULT_SPEED 5

class DrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                  byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin);

    void bearLeft(bool forward);

    void bearRight(bool forward);

    void turnLeft();

    void turnRight();

    void moveBackward();

    void moveForward();

    void stop();

    byte convertSpeed(byte speed);

    bool isMoving();

    void setSpeed(byte speed);

    void resetSpeed();

private:
    L293 _motorLeft;
    L293 _motorRight;
    bool _isMoving;
    byte _speed;
};

#endif