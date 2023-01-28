#ifndef DrivingModule_h
#define DrivingModule_h

#include "IDrivingModule.h"
#include <Arduino.h>
#include <L293.h>

const byte BEAR_SPEED_THRESHOLD = 3;

class DrivingModule : public IDrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftDirectionPin,
                  byte motorRightEnablePin, byte motorRightDirectionPin,
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

    Motion directionOfMotion() override;

    void setSpeed(byte speed) override;

    void resetSpeed() override;

private:
    L293_twoWire _motorLeft;
    L293_twoWire _motorRight;
    Motion _directionOfMotion;
    byte _speed;
    Stream *_stream;
};

#endif