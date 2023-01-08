#ifndef SensorModule_h
#define SensorModule_h

#include "ISensorModule.h"

class SensorModule : public ISensorModule
{
public:
    SensorModule(byte i2cAddress, byte interruptPin, Stream *stream);
    void begin() override;
    byte detect(SensorResult *r) override;
    bool signalled() override;

private:
    byte _i2cAddress;
    byte _interruptPin;
    byte _sensorBuffer[6];
    Stream *_stream;
};

#endif