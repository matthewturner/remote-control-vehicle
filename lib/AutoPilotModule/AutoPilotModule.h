#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "ISensorModule.h"
#include "IDrivingModule.h"

const byte SIDE_SENSOR_PADDING = 3;
const byte SIDE_SENSOR_COLLISION_THRESHOLD = 7 + SIDE_SENSOR_PADDING;
const byte SIDE_SENSOR_COLLISION_WARNING_THRESHOLD = 10 + SIDE_SENSOR_PADDING;
const byte FRONT_SENSOR_COLLISION_THRESHOLD = 10;
const byte SIDE_SENSOR_CLEAR_THRESHOLD = 20 + SIDE_SENSOR_PADDING;
const byte CENTER_TOLERANCE = 2;
const byte MAX_SENSOR_AGE_FOR_TURN = 100;
const byte MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR = 10;
const byte MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD = 10;
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
    SensorResult *sensorResult();

private:
    Stream *_stream;
    IDrivingModule *_drivingModule;
    ISensorModule *_sensorModule;
    SensorResult _sensorResult;
    unsigned int _maxSensorResultAge;
    bool _enabled = false;

    void moveForward();
    void turnRight();
    void turnLeft();
    void bearRight();
    void bearLeft();
};

#endif