#include "SensorModule.h"
#include "Debug.h"

SensorModule::SensorModule(byte i2cAddress, byte servoControlPin,
                           Stream *stream)
{
    _i2cAddress = i2cAddress;
    _servoControlPin = servoControlPin;
    _stream = stream;
}

void SensorModule::begin()
{
    _servo.attach(_servoControlPin);
    _sensor.begin(_i2cAddress);
    _servo.write(positionFrom(_sequence[_desiredSequenceIndex]));
}

bool SensorModule::signalled()
{
    return false;
}

bool SensorModule::scan(SensorResult *r)
{
  Direction direction = _sequence[_desiredSequenceIndex];

  if (request(r, direction))
  {
    _desiredSequenceIndex++;
    _desiredSequenceIndex %= 4;

    return true;
  }

  return false;
}

bool SensorModule::request(SensorResult *r, Direction direction)
{
    unsigned long now = millis();

    unsigned long elapsed = now - _lastChange;

    if (elapsed > POSITION_DELAY && elapsed <= POSITION_DELAY + 10)
    {
        // wait until distance has been measured
        return false;
    }

    Direction currentDirection = _sequence[_currentSequenceIndex];

    if (direction != currentDirection)
    {
        _currentSequenceIndex = sequenceIndexFrom(direction);        
        byte desiredPosition = positionFrom(direction);
        _servo.write(desiredPosition);
        _lastChange = now;
        return false;
    }

    if (elapsed <= POSITION_DELAY)
    {
        return false;
    }

    return detect(r);
}

byte SensorModule::sequenceIndexFrom(Direction direction)
{
    for (byte i = 0; i < SEQUENCE_COUNT; i++)
    {
        if (_sequence[i] == direction)
        {
            return i;
        }
    }
    return 0;
}

byte SensorModule::positionFrom(Direction direction)
{
    return _positions[(byte)direction];
}

bool SensorModule::detect(SensorResult *r)
{
    _sensor.rangingTest(&_measure, false);

    uint16_t distanceMillimeter = 800;

    if (_measure.RangeStatus != 4)
    {
        distanceMillimeter = _measure.RangeMilliMeter - OFFSET;
    }

    Direction desiredDirection = _sequence[_desiredSequenceIndex];

    switch(desiredDirection)
    {
        case Direction::LEFT:
            r->Left.Distance = distanceMillimeter;
            r->Left.Timestamp = millis();
            break;
        case Direction::RIGHT:
            r->Right.Distance = distanceMillimeter;
            r->Right.Timestamp = millis();
            break;
        default:
            r->Front.Distance = distanceMillimeter;
            r->Front.Timestamp = millis();
            debugPrint(F("Front Distance: "));
            debugPrintln(r->Front.Distance);
            break;
    }

    return true;
}