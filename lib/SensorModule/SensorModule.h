#ifndef SensorModule_h
#define SensorModule_h

#include <Wire.h>
#include <Arduino.h>
#include "ISensorModule.h"
#include <Adafruit_VL53L0X.h>
#include <Servo.h>

const short POSITION_DELAY = 500;

class SensorModule : public ISensorModule
{
public:
    SensorModule(byte i2cAddress, byte servoControlPin, Stream *stream);
    void begin() override;
    byte detect(SensorResult *r) override;
    bool signalled() override;
    void scan(SensorResult *r) override;

private:
    byte _i2cAddress;
    byte _servoControlPin;
    Stream *_stream;
    Servo _servo;
    byte _desiredStage = 0;
    byte _stages[4] = {0, 90, 180, 90};
    unsigned long _lastChange = 0;
    Adafruit_VL53L0X _sensor = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t _measure;
};

#endif