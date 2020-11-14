#include <Arduino.h>
#include "LedModule.h"
#include "Debug.h"

LedModule::LedModule(byte ledRedPin, byte ledGreenPin, byte ledBluePin)
{
    _redPin = ledRedPin;
    _greenPin = ledGreenPin;
    _bluePin = ledBluePin;
    _leds[0] = _redPin;
    _leds[1] = _greenPin;
    _leds[2] = _bluePin;

    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);
}

void LedModule::turnOffAllLeds()
{
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        turnOff(_leds[i]);
    }
}

void LedModule::cycleThroughLeds()
{
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        turnOn(_leds[i]);
        delay(200);
        turnOff(_leds[i]);
    }
}

void LedModule::toggleOn(byte pin)
{
    turnOffAllLeds();

    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        if (pin == _leds[i])
        {
            turnOn(_leds[i]);
        }
    }
}

void LedModule::turnOn(byte pin)
{
    digitalWrite(pin, HIGH);
}

void LedModule::turnOff(byte pin)
{
    digitalWrite(pin, LOW);
}

byte LedModule::redPin()
{
    return _redPin;
}

byte LedModule::greenPin()
{
    return _greenPin;
}

byte LedModule::bluePin()
{
    return _bluePin;
}