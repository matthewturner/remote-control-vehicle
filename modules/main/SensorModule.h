#ifndef SensorModule_h
#define SensorModule_h

struct sensorResult
{
    byte Front;
    byte Back;
    byte Left;
    byte Right;
    byte Age;
};

typedef struct sensorResult SensorResult;

class SensorModule
{
public:
    SensorModule(byte i2cAddress, HardwareSerial *stream);
    void begin();
    byte detect(SensorResult *r);

private:
    int _i2cAddress;
    byte _sensorBuffer[6];
    HardwareSerial *_stream;
};

#endif