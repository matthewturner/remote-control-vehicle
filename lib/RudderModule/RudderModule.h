#ifndef RudderModule_h
#define RudderModule_h

#include <Servo.h>
#include <Arduino.h>

class RudderModule
{
public:
    RudderModule(byte rudderPin, Stream *stream);

    void ahead();

    void port();

    void starboard();

private:
    Stream *_stream;
    Servo _servo;
};

#endif