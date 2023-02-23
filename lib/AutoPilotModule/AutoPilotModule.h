#ifndef AutoPilotModule_h
#define AutoPilotModule_h

#include <Arduino.h>
#include "ISensorModule.h"
#include "IDrivingModule.h"

const byte SIDE_SENSOR_PADDING = 60;
const byte SIDE_SENSOR_COLLISION_THRESHOLD = 70 + SIDE_SENSOR_PADDING;
const byte SIDE_SENSOR_COLLISION_WARNING_THRESHOLD = 100 + SIDE_SENSOR_PADDING;
const byte FRONT_SENSOR_COLLISION_THRESHOLD = 100;
const byte SIDE_SENSOR_CLEAR_THRESHOLD = 195 + SIDE_SENSOR_PADDING;
const byte CENTER_TOLERANCE = 20;
const byte MAX_SENSOR_AGE_FOR_TURN = 100;
const byte MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR = 10;
const byte MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD = 10;
const short MAX_FRONT_SENSOR_RESULT_AGE = 2000;
const short MAX_SIDE_SENSOR_RESULT_AGE = 10000;

enum class State : byte
{
    DISABLED = 0,
    RESETTING = 1,
    SCANNING = 2,
    DECIDING = 3,
    REQUESTING = 4
};

class AutoPilotModule
{
public:
    AutoPilotModule(Stream *stream,
                    IDrivingModule *drivingModule,
                    ISensorModule *sensorModule,
                    SensorResult *sensorResult);

    bool enabled();
    void enable();
    void disable();
    void handle();
    void handle2();
    bool isCentered();
    bool spaceAhead();
    bool isTrapped();
    bool isOneSideClear();
    unsigned short maxSensorResultAge();

private:
    Stream *_stream;
    IDrivingModule *_drivingModule;
    ISensorModule *_sensorModule;
    SensorResult *_sensorResult;
    State _state = State::DISABLED;

    bool outOfDate();
    void handleResetting();
    void handleScanning();
    void handleDeciding();
    void handleRequesting();
};

#endif