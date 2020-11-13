#include <Arduino.h>
#include "LedModule.h"

LedModule::LedModule(byte ledRedPin, byte ledGreenPin, byte ledBluePin)
{
    _ledRedPin = ledRedPin;
    _ledGreenPin = ledGreenPin;
    _ledBluePin = ledBluePin;

    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);
}

void LedModule::turnOffAllLeds()
{
    // DBG("Turning off all leds...");
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        turnOff(leds[i]);
    }
}

void LedModule::cycleThroughLeds()
{
    DBG("Cycling through all leds...");
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        turnOn(leds[i]);
        delay(200);
        turnOff(leds[i]);
    }
}

void LedModule::toggleOn(byte pin)
{
    turnOffAllLeds();

    // DBG("Toggling on led...");

    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        if (pin == leds[i])
        {
            turnOn(leds[i]);
        }
    }
}

void LedModule::turnOn(byte pin)
{
    // DBG("Turning on led...");
    digitalWrite(pin, HIGH);
}

void LedModule::turnOff(byte pin)
{
    // DBG("Turning off led...");
    digitalWrite(pin, LOW);
}
