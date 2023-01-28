#ifndef DrivingModule_h
#define DrivingModule_h

#include "IDrivingModule.h"
#include <Arduino.h>
#include <L293.h>

const byte BEAR_SPEED_THRESHOLD = 3;

class DrivingModule : public IDrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                  byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin,
                  Stream *stream);

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
    Motion _directionOfMotion;
    byte _speed;
    Stream *_stream;
};

#endif