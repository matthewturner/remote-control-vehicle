#include <Arduino.h>
#include "RudderModule.h"
#include "Debug.h"

RudderModule::RudderModule(byte rudderPin, HardwareSerial *stream)
{
    _servo.attach(rudderPin);
    _stream = stream;
}

void loop()
{
    _servo.write(0);
}
