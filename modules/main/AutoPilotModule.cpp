#include <Arduino.h>
#include "AutoPilotModule.h"
#include "Debug.h"

AutoPilotModule::AutoPilotModule(HardwareSerial *stream,
                                 DrivingModule *drivingModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
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

    if (sensorResult->Front < 7)
    {
        if (_drivingModule->directionOfMotion() == MOVE_FORWARD)
        {
            _drivingModule->stop();
        }
    }
}