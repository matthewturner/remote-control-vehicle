#include <Wire.h>
#include <Arduino.h>

#define COLLISION_THRESHOLD 8
#define LEFT_COLLISION_PIN 10
#define RIGHT_COLLISION_PIN 8
#define FRONT_COLLISION_PIN 3

#define NUMBER_OF_SENSORS 3

byte payload[6];
byte readings[NUMBER_OF_SENSORS];

#define FRONT_INDEX 0
#define LEFT_INDEX 1
#define RIGHT_INDEX 2
#define BACK_INDEX 3
#define AGE_INDEX 4
#define SENTINEL_INDEX 5

#define SIGNAL_PIN 12

void setup()
{
  Serial.begin(9600);
  Wire.begin(8);

  pinMode(RIGHT_COLLISION_PIN, INPUT);
  pinMode(LEFT_COLLISION_PIN, INPUT);
  pinMode(FRONT_COLLISION_PIN, INPUT);

  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);

  payload[FRONT_INDEX] = -1;
  payload[LEFT_INDEX] = -1;
  payload[RIGHT_INDEX] = -1;
  payload[BACK_INDEX] = 100;
  payload[AGE_INDEX] = 250;
  payload[SENTINEL_INDEX] = 255;

  Wire.onRequest(requestEvent);
}

byte frontReading = 0;
byte leftReading = 0;
byte rightReading = 0;
bool signal = false;

void loop()
{
  frontReading = readFrom(FRONT_COLLISION_PIN);
  leftReading = readFrom(LEFT_COLLISION_PIN);
  rightReading = readFrom(RIGHT_COLLISION_PIN);

  if (readings[FRONT_INDEX] != frontReading)
  {
    readings[FRONT_INDEX] = frontReading;
    signal = true;
  }
  if (readings[LEFT_INDEX] != leftReading)
  {
    readings[LEFT_INDEX] = leftReading;
    signal = true;
  }
  if (readings[RIGHT_INDEX] != rightReading)
  {
    readings[RIGHT_INDEX] = rightReading;
    signal = true;
  }

  if (signal)
  {
    digitalWrite(SIGNAL_PIN, HIGH);
    signal = false;
  }
}

void requestEvent()
{
  payload[FRONT_INDEX] = readings[FRONT_INDEX];
  payload[LEFT_INDEX] = readings[LEFT_INDEX];
  payload[RIGHT_INDEX] = readings[RIGHT_INDEX];

  payload[AGE_INDEX] = 0;

  Wire.write(payload, 6);

  digitalWrite(SIGNAL_PIN, LOW);
}

byte readFrom(byte pin)
{
  if (digitalRead(pin) == HIGH)
  {
    return 100;
  }
  else
  {
    return COLLISION_THRESHOLD;
  }
}