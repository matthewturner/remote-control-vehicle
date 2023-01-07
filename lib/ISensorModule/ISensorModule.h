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
    virtual void begin();
    virtual byte detect(SensorResult *r);
    virtual bool signalled();
};

#endif