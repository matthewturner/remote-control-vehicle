#ifndef SensorModule_h
#define SensorModule_h

#include <Wire.h>
#include <Arduino.h>
#include "ISensorModule.h"
#include <Adafruit_VL53L0X.h>
#include <Servo.h>

const short POSITION_DELAY = 400;
const short OFFSET = 50;
const byte SEQUENCE_COUNT = 4;

class SensorModule : public ISensorModule
{
public:
    SensorModule(byte i2cAddress, byte servoControlPin, Stream *stream);
    void begin() override;
    void reset() override;
    bool detect(SensorResult *r) override;
    bool signalled() override;
    bool scan(SensorResult *r) override;
    bool scanOnce(SensorResult *r) override;
    bool request(SensorResult *r, Direction direction) override;

private:
    byte positionFrom(Direction direction);
    byte sequenceIndexFrom(Direction direction);
    
    byte _i2cAddress;
    byte _servoControlPin;
    Stream *_stream;
    Servo _servo;
    byte _desiredSequenceIndex = 1;
    byte _currentSequenceIndex = 0;
    Direction _sequence[SEQUENCE_COUNT] = { Direction::LEFT, Direction::FRONT, Direction::RIGHT, Direction::FRONT };
    byte _positions[SEQUENCE_COUNT] = {90, 90, 180, 0};
    unsigned long _lastChange = 0;
    Adafruit_VL53L0X _sensor = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t _measure;
};

#endif