#ifndef SensorModule_h
#define SensorModule_h

#include <Wire.h>
#include <Arduino.h>
#include "ISensorModule.h"
#include <Adafruit_VL53L0X.h>
#include <Servo.h>

class SensorModule : public ISensorModule
{
public:
    SensorModule(byte i2cAddress, byte servoControlPin, Stream *stream);
    void begin() override;
    byte detect(SensorResult *r) override;
    bool signalled() override;

private:
    byte _i2cAddress;
    byte _servoControlPin;
    byte _sensorBuffer[6];
    Stream *_stream;
    Servo _servo;
    Adafruit_VL53L0X _sensor = Adafruit_VL53L0X();
    // VL53L0X_RangingMeasurementData_t _measure;
};

#endif