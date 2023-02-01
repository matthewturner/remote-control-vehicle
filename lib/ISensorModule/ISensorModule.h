#ifndef ISensorModule_h
#define ISensorModule_h

#include <Arduino.h>

typedef struct sensorResult
{
    uint16_t Front;
    uint16_t Left;
    uint16_t Right;
    uint16_t Age;
} SensorResult;

class ISensorModule
{
public:
    ISensorModule();
    virtual void begin() = 0;
    virtual byte detect(SensorResult *r) = 0;
    virtual bool signalled() = 0;
    virtual void scan(SensorResult *r) = 0;
};

#endif