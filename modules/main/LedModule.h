#ifndef LedModule_h
#define LedModule_h

#define NUMBER_OF_LEDS 3

class LedModule
{
public:
    LedModule(byte ledRedPin, byte ledGreenPin, byte ledBluePin);

    void turnOffAllLeds();

    void cycleThroughLeds();

    void toggleOn(byte pin);

    void turnOn(byte pin);

    void turnOff(byte pin);

    byte redPin();

    byte greenPin();

    byte bluePin();

private:
    byte _redPin;
    byte _greenPin;
    byte _bluePin;
    byte _leds[3];
};

#endif