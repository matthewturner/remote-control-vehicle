#include <Wire.h>

#include <HCSR04.h>

HCSR04 sensors(2, new int[3]{5, 6, 7}, 3);
byte distances[6];
unsigned long lastReading;

#define FRONT_INDEX 0
#define LEFT_INDEX 1
#define RIGHT_INDEX 2
#define BACK_INDEX 3
#define AGE_INDEX 4
#define SENTINEL_INDEX 5

void setup()
{
  Wire.begin(8);

  distances[FRONT_INDEX] = -1;
  distances[LEFT_INDEX] = -1;
  distances[RIGHT_INDEX] = -1;
  distances[BACK_INDEX] = 100;
  distances[AGE_INDEX] = 250;
  distances[SENTINEL_INDEX] = 255;

  Wire.onRequest(requestEvent);
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    distances[i] = sensors.dist(i);
  }

  lastReading = millis();
  delay(100);
}

void requestEvent()
{
  unsigned long time = millis();
  unsigned long age = time - lastReading;
  if (age > 250)
  {
    age = 250;
  }
  distances[AGE_INDEX] = age;
  Wire.write(distances, 6);
}
