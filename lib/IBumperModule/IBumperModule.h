#ifndef IBumperModule_h
#define IBumperModule_h

#include <Arduino.h>

enum class Sides : byte
{
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    BOTH = LEFT | RIGHT
};

class IBumperModule
{
public:
    IBumperModule();

    virtual void begin() = 0;

    virtual Sides hasCollided(unsigned short sinceMs = 0) = 0;
};

#endif