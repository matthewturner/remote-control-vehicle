#ifndef BoatDrivingModule_h
#define BoatDrivingModule_h

#include <Arduino.h>
#include "IDrivingModule.h"
#include "DrivingModule.h"
#include "RudderModule.h"

class BoatDrivingModule : public IDrivingModule
{
public:
    BoatDrivingModule(IDrivingModule *drivingModule, RudderModule *rudderModule);

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
    IDrivingModule *_drivingModule;
    RudderModule *_rudderModule;
};

#endif
