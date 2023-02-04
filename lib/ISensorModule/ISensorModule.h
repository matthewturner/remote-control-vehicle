#ifndef ISensorModule_h
#define ISensorModule_h

#include <Arduino.h>

enum class Direction : byte
{
    NONE = 0,
    FRONT = 1,
    LEFT = 2,
    RIGHT = 3
};

typedef struct sensorResultDetail
{
    uint16_t Distance;
    unsigned long Timestamp;
} SensorResultDetail;

typedef struct sensorResult
{
    SensorResultDetail Front;
    SensorResultDetail Left;
    SensorResultDetail Right;
} SensorResult;

class ISensorModule
{
public:
    ISensorModule();
    virtual void begin() = 0;
    virtual bool detect(SensorResult *r) = 0;
    virtual bool signalled() = 0;
    virtual bool scan(SensorResult *r) = 0;
    virtual bool request(SensorResult *r, Direction direction) = 0;
};

#endif