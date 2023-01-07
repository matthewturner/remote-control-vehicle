#include "RudderModule.h"
#include "Debug.h"

RudderModule::RudderModule(byte rudderPin, Stream *stream)
{
    _servo.attach(rudderPin);
    _stream = stream;
}

void RudderModule::ahead()
{
    _servo.write(90);
}

void RudderModule::port()
{
    _servo.write(0);
}

void RudderModule::starboard()
{
    _servo.write(180);
}