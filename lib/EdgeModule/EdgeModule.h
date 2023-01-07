#ifndef EdgeModule_h
#define EdgeModule_h

#include <Arduino.h>

const byte DEFAULT_DURATION = 200;

class EdgeModule
{
public:
    EdgeModule(Stream *stream);
    void increase();
    void decrease();
    void reset();
    short duration();

private:
    short _duration;
    Stream *_stream;
};

#endif