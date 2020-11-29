#include <Wire.h>

#include <HCSR04.h>

#define SIGNAL_THRESHOLD 7

#define NUMBER_OF_SENSORS 3
#define NUMBER_OF_READINGS 1

HCSR04 sensors(8, new int[NUMBER_OF_SENSORS]{9, 10, 11}, NUMBER_OF_SENSORS);
byte payload[6];
byte readings[NUMBER_OF_SENSORS][NUMBER_OF_READINGS];
unsigned long lastReading;
byte readingIndex = 0;

#define MAX_SENSOR_DISTANCE 250

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

  delay(100);
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
}

byte readFrom(byte sensorIndex, byte readingIndex)
{
  byte reading = min(sensors.dist(sensorIndex), MAX_SENSOR_DISTANCE);
  if (reading == 0)
  {
    Serial.println("Skipping reading...");
  }
  else
  {
    readings[sensorIndex][readingIndex] = reading;
    if (reading <= SIGNAL_THRESHOLD)
    {
      digitalWrite(SIGNAL_PIN, HIGH);
    }
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