#ifndef ISensorModule_h
#define ISensorModule_h

#include <Arduino.h>

struct sensorResult
{
    byte Front;
    byte Back;
    byte Left;
    byte Right;
    byte Age;
};

typedef struct sensorResult SensorResult;

class ISensorModule
{
public:
    ISensorModule();
    virtual void begin() = 0;
    virtual byte detect(SensorResult *r) = 0;
    virtual bool signalled() = 0;
};

#endif