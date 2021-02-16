#include <Wire.h>

#include <HCSR04.h>

#define SIGNAL_THRESHOLD 7
#define COLLISION_THRESHOLD 7
#define WARNING_THRESHOLD 14
#define PENDING_DATA_PIN 13

#define NUMBER_OF_SENSORS 3
#define NUMBER_OF_READINGS 1
#define AVERAGE_READING_INDEX NUMBER_OF_READINGS

HCSR04 sensors(8, new int[NUMBER_OF_SENSORS]{9, 10, 11}, NUMBER_OF_SENSORS);
byte payload[6];
byte readings[NUMBER_OF_SENSORS][NUMBER_OF_READINGS + 1];
unsigned long lastReading;
byte readingIndex = 0;

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
  readFrom(FRONT_INDEX, readingIndex);
  readFrom(LEFT_INDEX, readingIndex);
  readFrom(RIGHT_INDEX, readingIndex);

  // Serial.print(readings[LEFT_INDEX][readingIndex]);
  // Serial.print("cm <-- ^ ");
  // Serial.print(readings[FRONT_INDEX][readingIndex]);
  // Serial.print("cm ^ --> ");
  // Serial.print(readings[RIGHT_INDEX][readingIndex]);
  // Serial.println("cm");

  readingIndex++;
  if (readingIndex >= NUMBER_OF_READINGS)
  {
    readingIndex = 0;
  }

  lastReading = millis();

  delay(50);
}

void requestEvent()
{
  payload[FRONT_INDEX] = average(readings[FRONT_INDEX]);
  payload[LEFT_INDEX] = average(readings[LEFT_INDEX]);
  payload[RIGHT_INDEX] = average(readings[RIGHT_INDEX]);

  unsigned long time = millis();
  unsigned long age = min(time - lastReading, 250);
  payload[AGE_INDEX] = (byte)age;

  Wire.write(payload, 6);

  digitalWrite(SIGNAL_PIN, LOW);
  digitalWrite(PENDING_DATA_PIN, LOW);
}

byte readFrom(byte sensorIndex, byte readingIndex)
{
  byte reading = min(sensors.dist(sensorIndex), MAX_SENSOR_DISTANCE);
  if (reading == 0)
  {
    Serial.println("Skipping reading...");
    return;
  }

  byte previousAverageReading = readings[sensorIndex][AVERAGE_READING_INDEX];
  readings[sensorIndex][readingIndex] = reading;
  byte averageReading = average(readings[sensorIndex]);
  readings[sensorIndex][AVERAGE_READING_INDEX] = averageReading;

  if (averageReading <= COLLISION_THRESHOLD)
  {
    digitalWrite(indicators[sensorIndex][WARNING_INDEX], LOW);
    digitalWrite(indicators[sensorIndex][COLLISION_INDEX], HIGH);
    if (previousAverageReading > COLLISION_THRESHOLD)
    {
      digitalWrite(SIGNAL_PIN, HIGH);
      digitalWrite(PENDING_DATA_PIN, HIGH);
    }
  }
  else if (averageReading <= WARNING_THRESHOLD)
  {
    digitalWrite(indicators[sensorIndex][COLLISION_INDEX], LOW);
    digitalWrite(indicators[sensorIndex][WARNING_INDEX], HIGH);
    if (previousAverageReading > WARNING_THRESHOLD)
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

byte average(byte readingArray[])
{
  int sum = 0;
  for (byte i = 0; i < NUMBER_OF_READINGS; i++)
  {
    sum += readingArray[i];
  }
  return (byte)(sum / NUMBER_OF_READINGS);
}