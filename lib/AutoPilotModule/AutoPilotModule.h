#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "SensorModule.h"
#include "DrivingModule.h"

#define SIDE_SENSOR_PADDING 3
#define SIDE_SENSOR_COLLISION_THRESHOLD 7 + SIDE_SENSOR_PADDING
#define SIDE_SENSOR_COLLISION_WARNING_THRESHOLD 10 + SIDE_SENSOR_PADDING
#define FRONT_SENSOR_COLLISION_THRESHOLD 10
#define SIDE_SENSOR_CLEAR_THRESHOLD 20 + SIDE_SENSOR_PADDING
#define CENTER_TOLERANCE 2
#define MAX_SENSOR_RESULT_AGE 3000
#define MAX_SENSOR_AGE_FOR_TURN 100
#define MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR 10
#define MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD 10

class AutoPilotModule
{
public:
    AutoPilotModule(Stream *stream,
                    IDrivingModule *drivingModule,
                    SensorModule *sensorModule);

    bool enabled();
    void enable();
    void disable();
    void handle();

private:
    Stream *_stream;
    IDrivingModule *_drivingModule;
    SensorModule *_sensorModule;
    SensorResult _sensorResult;
    unsigned long _sampleAge;
    unsigned int _maxSensorResultAge;
    bool _enabled = false;

    bool isCentered(SensorResult *sensorResult);
    bool spaceAhead(SensorResult *sensorResult);
    bool isTrapped(SensorResult *sensorResult);
    bool isOneSideClear(SensorResult *sensorResult);
};

#endif