#include <Arduino.h>
#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(HardwareSerial *stream,
                                 DrivingModule *drivingModule,
                                 CommandModule *commandModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _commandModule = commandModule;
}

void AutoPilotModule::handle(SensorResult *sensorResult)
{
    DBGP("Sensor Module Result:");
    DBGP_PRNT("   ");
    DBGP(sensorResult->Front);
    DBGP_PRNT(sensorResult->Left);
    DBGP_PRNT("       ");
    DBGP(sensorResult->Right);
    DBGP_PRNT("   ");
    DBGP(sensorResult->Back);
    DBGP_PRNT("   ~");
    DBGP(sensorResult->Age);

    if (sensorResult->Front <= 7)
    {
        if (!_commandModule->selfDriveMode())
        {
            if (_drivingModule->directionOfMotion() == MOVE_FORWARD)
            {
                _drivingModule->stop();
            }
            return;
        }
        if (sensorResult->Right > sensorResult->Left)
        {
            _drivingModule->turnRight();
        }
        else if (sensorResult->Left > sensorResult->Right)
        {
            _drivingModule->turnLeft();
        }
    }
    else if (sensorResult->Front > 7)
    {
        if (sensorResult->Left < 4)
        {
            _drivingModule->turnRight();
        }
        else if (sensorResult->Right < 4)
        {
            _drivingModule->turnLeft();
        }
        else if (sensorResult->Left < 7)
        {
            _drivingModule->bearRight(DIR_FORWARD);
        }
        else if (sensorResult->Right < 7)
        {
            _drivingModule->bearLeft(DIR_FORWARD);
        }
        else if (sensorResult->Left - 2 < sensorResult->Right < sensorResult->Left + 2)
        {
            _drivingModule->moveForward();
        }
        else if (sensorResult->Right - 2 < sensorResult->Left < sensorResult->Right + 2)
        {
            _drivingModule->moveForward();
        }
        else if (sensorResult->Left > 20 || sensorResult->Right > 20)
        {
            _drivingModule->moveForward();
        }
        else if (sensorResult->Left < sensorResult->Right)
        {
            _drivingModule->bearRight(DIR_FORWARD);
        }
        else
        {
            _drivingModule->bearLeft(DIR_FORWARD);
        }
    }
}