#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "ISensorModule.h"
#include "IDrivingModule.h"

#define SIDE_SENSOR_PADDING 3
#define SIDE_SENSOR_COLLISION_THRESHOLD 7 + SIDE_SENSOR_PADDING
#define SIDE_SENSOR_COLLISION_WARNING_THRESHOLD 10 + SIDE_SENSOR_PADDING
#define FRONT_SENSOR_COLLISION_THRESHOLD 10
#define SIDE_SENSOR_CLEAR_THRESHOLD 20 + SIDE_SENSOR_PADDING
#define CENTER_TOLERANCE 2
#define MAX_SENSOR_AGE_FOR_TURN 100
#define MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR 10
#define MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD 10
const short MAX_SENSOR_RESULT_AGE = 3000;

class AutoPilotModule
{
public:
    AutoPilotModule(Stream *stream,
                    IDrivingModule *drivingModule,
                    ISensorModule *sensorModule);

    bool enabled();
    void enable();
    void disable();
    void handle();
    bool isCentered();
    bool spaceAhead();
    bool isTrapped();
    bool isOneSideClear();
    bool updatePositionIfRequired();
    unsigned long resultAge();
    unsigned int maxSensorResultAge();
    void updateResult(SensorResult *result);

private:
    Stream *_stream;
    IDrivingModule *_drivingModule;
    ISensorModule *_sensorModule;
    SensorResult _sensorResult;
    unsigned long _resultAge;
    unsigned int _maxSensorResultAge;
    bool _enabled = false;
};

#endif