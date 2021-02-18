#include <Wire.h>

#include <HCSR04.h>

#define COLLISION_THRESHOLD 10
#define WARNING_THRESHOLD 20
#define PENDING_DATA_PIN 13

#define NUMBER_OF_SENSORS 3

HCSR04 sensors(8, new int[NUMBER_OF_SENSORS]{9, 10, 11}, NUMBER_OF_SENSORS);
byte payload[6];
byte readings[NUMBER_OF_SENSORS];
unsigned long lastReading;

#define MAX_SENSOR_DISTANCE 250

#define FRONT_INDEX 0
#define LEFT_INDEX 1
#define RIGHT_INDEX 2
#define BACK_INDEX 3
#define AGE_INDEX 4
#define SENTINEL_INDEX 5

#define COLLISION_INDEX 0
#define WARNING_INDEX 1
#define FRONT_WARNING_PIN 2
#define FRONT_COLLISION_PIN 3
#define RIGHT_WARNING_PIN 4
#define RIGHT_COLLISION_PIN 5
#define LEFT_WARNING_PIN 6
#define LEFT_COLLISION_PIN 7
byte indicators[NUMBER_OF_SENSORS][2];

#define SIGNAL_PIN 12

void setup()
{
  Serial.begin(9600);
  Wire.begin(8);

  pinMode(RIGHT_WARNING_PIN, OUTPUT);
  pinMode(RIGHT_COLLISION_PIN, OUTPUT);
  pinMode(LEFT_WARNING_PIN, OUTPUT);
  pinMode(LEFT_COLLISION_PIN, OUTPUT);
  pinMode(FRONT_WARNING_PIN, OUTPUT);
  pinMode(FRONT_COLLISION_PIN, OUTPUT);
  pinMode(PENDING_DATA_PIN, OUTPUT);

  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);

  indicators[RIGHT_INDEX][WARNING_INDEX] = RIGHT_WARNING_PIN;
  indicators[RIGHT_INDEX][COLLISION_INDEX] = RIGHT_COLLISION_PIN;
  indicators[LEFT_INDEX][WARNING_INDEX] = LEFT_WARNING_PIN;
  indicators[LEFT_INDEX][COLLISION_INDEX] = LEFT_COLLISION_PIN;
  indicators[FRONT_INDEX][WARNING_INDEX] = FRONT_WARNING_PIN;
  indicators[FRONT_INDEX][COLLISION_INDEX] = FRONT_COLLISION_PIN;

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
  readFrom(FRONT_INDEX);
  readFrom(RIGHT_INDEX);
  readFrom(LEFT_INDEX);

  // Serial.print(readings[LEFT_INDEX][readingIndex]);
  // Serial.print("cm <-- ^ ");
  // Serial.print(readings[FRONT_INDEX][readingIndex]);
  // Serial.print("cm ^ --> ");
  // Serial.print(readings[RIGHT_INDEX][readingIndex]);
  // Serial.println("cm");

  lastReading = millis();
}

void requestEvent()
{
  payload[FRONT_INDEX] = readings[FRONT_INDEX];
  payload[LEFT_INDEX] = readings[LEFT_INDEX];
  payload[RIGHT_INDEX] = readings[RIGHT_INDEX];

  unsigned long time = millis();
  unsigned long age = min(time - lastReading, 250);
  payload[AGE_INDEX] = (byte)age;

  Wire.write(payload, 6);

  digitalWrite(SIGNAL_PIN, LOW);
  digitalWrite(PENDING_DATA_PIN, LOW);
}

byte readFrom(byte sensorIndex)
{
  byte reading = min(sensors.dist(sensorIndex), MAX_SENSOR_DISTANCE);
  if (reading == 0)
  {
    Serial.println("Skipping reading...");
    return;
  }

  byte previousReading = readings[sensorIndex];
  readings[sensorIndex] = reading;

  if (reading <= COLLISION_THRESHOLD)
  {
    digitalWrite(indicators[sensorIndex][WARNING_INDEX], LOW);
    digitalWrite(indicators[sensorIndex][COLLISION_INDEX], HIGH);
    if (previousReading > COLLISION_THRESHOLD)
    {
      digitalWrite(SIGNAL_PIN, HIGH);
      digitalWrite(PENDING_DATA_PIN, HIGH);
    }
  }
  else if (reading <= WARNING_THRESHOLD)
  {
    digitalWrite(indicators[sensorIndex][COLLISION_INDEX], LOW);
    digitalWrite(indicators[sensorIndex][WARNING_INDEX], HIGH);
    if (previousReading > WARNING_THRESHOLD)
    {
      digitalWrite(SIGNAL_PIN, HIGH);
      digitalWrite(PENDING_DATA_PIN, HIGH);
    }
  }
  else
  {
    digitalWrite(indicators[sensorIndex][WARNING_INDEX], LOW);
    digitalWrite(indicators[sensorIndex][COLLISION_INDEX], LOW);
  }
}