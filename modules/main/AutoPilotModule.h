#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "SensorModule.h"
#include "DrivingModule.h"

class AutoPilotModule
{
public:
    AutoPilotModule(HardwareSerial *stream,
                    DrivingModule *drivingModule);

    void handle(SensorResult *sensorResult);

private:
    HardwareSerial *_stream;
    DrivingModule *_drivingModule;
};

#endif