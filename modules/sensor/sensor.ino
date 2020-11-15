#include <Wire.h>

#include <HCSR04.h>

#define NUMBER_OF_SENSORS 2

HCSR04 sensors(8, new int[NUMBER_OF_SENSORS]{10, 11}, NUMBER_OF_SENSORS);
byte distances[6];
unsigned long lastReading;

#define SENSOR_FRONT_INDEX 0
#define SENSOR_LEFT_INDEX 0
#define SENSOR_RIGHT_INDEX 1
#define SENSOR_BACK_INDEX 0

#define MAX_SENSOR_DISTANCE 250

#define FRONT_INDEX 0
#define LEFT_INDEX 1
#define RIGHT_INDEX 2
#define BACK_INDEX 3
#define AGE_INDEX 4
#define SENTINEL_INDEX 5

void setup()
{
  Serial.begin(9600);
  Wire.begin(8);

  distances[FRONT_INDEX] = 100;
  distances[LEFT_INDEX] = -1;
  distances[RIGHT_INDEX] = -1;
  distances[BACK_INDEX] = 100;
  distances[AGE_INDEX] = 250;
  distances[SENTINEL_INDEX] = 255;

  Wire.onRequest(requestEvent);
}

void loop()
{
  distances[LEFT_INDEX] = min(sensors.dist(SENSOR_LEFT_INDEX), MAX_SENSOR_DISTANCE);
  distances[RIGHT_INDEX] = min(sensors.dist(SENSOR_RIGHT_INDEX), MAX_SENSOR_DISTANCE);

  Serial.print("Left: ");
  Serial.print(distances[LEFT_INDEX]);
  Serial.print("cm; Right: ");
  Serial.print(distances[RIGHT_INDEX]);
  Serial.println("cm");

  lastReading = millis();
  delay(1000);
}

void requestEvent()
{
  unsigned long time = millis();
  unsigned long age = min(time - lastReading, 250);
  distances[AGE_INDEX] = age;
  Wire.write(distances, 6);
}
