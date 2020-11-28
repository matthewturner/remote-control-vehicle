#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "SensorModule.h"
#include "DrivingModule.h"
#include "CommandModule.h"

#define SIDE_SENSOR_PADDING 3
#define SIDE_SENSOR_COLLISION_THRESHOLD 7 + SIDE_SENSOR_PADDING
#define SIDE_SENSOR_COLLISION_WARNING_THRESHOLD 10 + SIDE_SENSOR_PADDING
#define FRONT_SENSOR_COLLISION_THRESHOLD 10
#define SIDE_SENSOR_CLEAR_THRESHOLD 20 + SIDE_SENSOR_PADDING
#define CENTER_TOLERANCE 2
#define MAX_SENSOR_RESULT_AGE 300

class AutoPilotModule
{
public:
    AutoPilotModule(HardwareSerial *stream,
                    DrivingModule *drivingModule,
                    CommandModule *commandModule,
                    SensorModule *sensorModule);

    void handle();

private:
    HardwareSerial *_stream;
    DrivingModule *_drivingModule;
    CommandModule *_commandModule;
    SensorModule *_sensorModule;
    SensorResult _sensorResult;
    unsigned long _sampleAge;

    bool isCentered(SensorResult *sensorResult);
    bool spaceAhead(SensorResult *sensorResult);
    bool isTrapped(SensorResult *sensorResult);
    bool isOneSideClear(SensorResult *sensorResult);
};

#endif