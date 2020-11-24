#include <Wire.h>

#include <HCSR04.h>

#define NUMBER_OF_SENSORS 3

HCSR04 sensors(8, new int[NUMBER_OF_SENSORS]{9, 10, 11}, NUMBER_OF_SENSORS);
byte payload[6];
unsigned long lastReading;

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

  payload[FRONT_INDEX] = -1;
  payload[LEFT_INDEX] = -1;
  payload[RIGHT_INDEX] = -1;
  payload[BACK_INDEX] = 100;
  payload[AGE_INDEX] = 250;
  payload[SENTINEL_INDEX] = 255;

  Wire.onRequest(requestEvent);
}

void loop()
{
  payload[FRONT_INDEX] = min(sensors.dist(FRONT_INDEX), MAX_SENSOR_DISTANCE);
  payload[LEFT_INDEX] = min(sensors.dist(LEFT_INDEX), MAX_SENSOR_DISTANCE);
  payload[RIGHT_INDEX] = min(sensors.dist(RIGHT_INDEX), MAX_SENSOR_DISTANCE);

  Serial.print(payload[LEFT_INDEX]);
  Serial.print("cm <-- ^ ");
  Serial.print(payload[FRONT_INDEX]);
  Serial.print("cm ^ --> ");
  Serial.print(payload[RIGHT_INDEX]);
  Serial.println("cm");

  lastReading = millis();
  delay(100);
}

void requestEvent()
{
  unsigned long time = millis();
  unsigned long age = min(time - lastReading, 250);
  payload[AGE_INDEX] = (byte)age;
  Wire.write(payload, 6);
}
