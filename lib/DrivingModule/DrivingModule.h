#ifndef DrivingModule_h
#define DrivingModule_h

#include "IDrivingModule.h"
#include <L293.h>

class DrivingModule : public IDrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                  byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin,
                  HardwareSerial *stream);

    void bearLeft(bool forward) override;

    void bearRight(bool forward) override;

    void turnLeft() override;

    void turnRight() override;

    void moveBackward() override;

    void moveForward() override;

    void stop() override;

    byte convertSpeed(byte speed) override;

    bool isMoving() override;

    byte directionOfMotion() override;

    void setSpeed(byte speed) override;

    void resetSpeed() override;

private:
    L293 _motorLeft;
    L293 _motorRight;
    byte _directionOfMotion;
    byte _speed;
    HardwareSerial *_stream;
};

#endif