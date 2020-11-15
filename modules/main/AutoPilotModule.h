#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "SensorModule.h"
#include "DrivingModule.h"
#include "CommandModule.h"

class AutoPilotModule
{
public:
    AutoPilotModule(HardwareSerial *stream,
                    DrivingModule *drivingModule,
                    CommandModule *commandModule);

    void handle(SensorResult *sensorResult);

private:
    HardwareSerial *_stream;
    DrivingModule *_drivingModule;
    CommandModule *_commandModule;

    bool isCentered(SensorResult *sensorResult);
    bool spaceAhead(SensorResult *sensorResult);
    bool isTrapped(SensorResult *sensorResult);
};

#endif