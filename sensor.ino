#include "Adafruit_VL53L0X.h"
#include <Servo.h>

#define LOX1_ADDRESS 0x30

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo servo;


void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  servo.attach(6);

  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

  // start continuous ranging
  // lox.startRangeContinuous();
}

const short POSITION_DELAY = 200;
const short MESSAGE_DELAY = 300;

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.println("Facing right (0)");
  Serial.print("Right Distance in mm: ");
  servo.write(0);
  delay(POSITION_DELAY);
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  delay(MESSAGE_DELAY);

  Serial.println("Facing forward (90)");
  Serial.print("Front Distance in mm: ");
  servo.write(90);
  delay(POSITION_DELAY);
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  delay(MESSAGE_DELAY);

  Serial.println("Facing left (180)");
  Serial.print("Left Distance in mm: ");
  servo.write(180);
  delay(POSITION_DELAY);
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  delay(MESSAGE_DELAY);
  
  Serial.println("Facing forward (90)");
  Serial.print("Front Distance in mm: ");
  servo.write(90);
  delay(POSITION_DELAY);
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  delay(MESSAGE_DELAY);
}
