#ifndef SensorModule_h
#define SensorModule_h

struct sensorResult
{
    byte Forward;
    byte Back;
    byte Left;
    byte Right;
    byte Age;
};

typedef struct sensorResult SensorResult;

class SensorModule
{
public:
    SensorModule(byte i2cAddress);
    void begin();
    SensorResult detect();

private:
    int _i2cAddress;
    byte _sensorBuffer[6];
};

#endif