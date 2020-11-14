#ifndef DrivingModule_h
#define DrivingModule_h

#include <L293.h>

#define DIR_FORWARD true
#define DIR_REVERSE false

#define MOVE_STOPPED 0
#define MOVE_FORWARD 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3
#define MOVE_BACKWARD 4

#define DEFAULT_SPEED 5

class DrivingModule
{
public:
    DrivingModule(byte motorLeftEnablePin, byte motorLeftForwardPin, byte motorLeftReversePin,
                  byte motorRightEnablePin, byte motorRightForwardPin, byte motorRightReversePin,
                  HardwareSerial *stream);

    void bearLeft(bool forward);

    void bearRight(bool forward);

    void turnLeft();

    void turnRight();

    void moveBackward();

    void moveForward();

    void stop();

    byte convertSpeed(byte speed);

    bool isMoving();

    byte directionOfMotion();

    void setSpeed(byte speed);

    void resetSpeed();

private:
    L293 _motorLeft;
    L293 _motorRight;
    byte _directionOfMotion;
    byte _speed;
    HardwareSerial *_stream;
};

#endif