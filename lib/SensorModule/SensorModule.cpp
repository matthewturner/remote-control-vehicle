#include "SensorModule.h"

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

    Direction currentDirection = _sequence[_currentSequenceIndex];

    if (direction != currentDirection)
    {
        _currentSequenceIndex = sequenceIndexFrom(direction);        
        byte desiredPosition = positionFrom(direction);
        _servo.write(desiredPosition);
        _lastChange = now;
        return false;
    }

    unsigned long elapsed = now - _lastChange;

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

    if (_measure.RangeStatus == 4)
    {
        // out of range
        return true;
    }

    Direction desiredDirection = _sequence[_desiredSequenceIndex];

    switch(desiredDirection)
    {
        case Direction::LEFT:
            r->Left.Distance = _measure.RangeMilliMeter - OFFSET;
            r->Left.Timestamp = millis();
            break;
        case Direction::RIGHT:
            r->Right.Distance = _measure.RangeMilliMeter - OFFSET;
            r->Right.Timestamp = millis();
            break;
        default:
            r->Front.Distance = _measure.RangeMilliMeter - OFFSET;
            r->Front.Timestamp = millis();
            break;
    }

    return true;
}