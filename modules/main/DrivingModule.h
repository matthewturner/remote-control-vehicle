#ifndef DrivingModule_h
#define DrivingModule_h

#include <L293.h>

#define DIR_FORWARD true
#define DIR_REVERSE false

class DrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                  byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin);

    void bearLeft(byte speed, bool forward);

    void bearRight(byte speed, bool forward);

    void turnLeft(byte speed);

    void turnRight(byte speed);

    void moveBackward(byte speed);

    void moveForward(byte speed);

    void stop();

    byte convertSpeed(byte speed);

    bool isMoving();

private:
    L293 _motorLeft;
    L293 _motorRight;
    bool _isMoving;
};

#endif