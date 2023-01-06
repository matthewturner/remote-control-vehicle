#include <Arduino.h>
#include "ControlModule.h"
#include "Debug.h"

ControlModule::ControlModule(HardwareSerial *stream, IDrivingModule *drivingModule,
                             EdgeModule *edgeModule,
                             LedModule *ledModule, CommandModule *commandModule)
{
    _stream = stream;
    _drivingModule = drivingModule;
    _edgeModule = edgeModule;
    _ledModule = ledModule;
    _commandModule = commandModule;
}

void ControlModule::executeInstruction(int instruction)
{
    if (instruction == -1)
    {
        return;
    }

    DBGP_PRNT("Executing instruction ");
    DBGP_PRNT(instruction, HEX);
    DBGP("...");

    switch (instruction)
    {
    case DELAY:
        _awaitingDelayInstruction = true;
        break;
    case SET_SPEED_1:
        _drivingModule->setSpeed(1);
        break;
    case SET_SPEED_2:
        _drivingModule->setSpeed(2);
        break;
    case SET_SPEED_3:
        _drivingModule->setSpeed(3);
        break;
    case SET_SPEED_4:
        _drivingModule->setSpeed(4);
        break;
    case SET_SPEED_5:
        _drivingModule->setSpeed(5);
        break;
    case FORWARD:
        _drivingModule->moveForward();
        break;
    case RIGHT:
        _drivingModule->turnRight();
        break;
    case LEFT:
        _drivingModule->turnLeft();
        break;
    case REVERSE:
        _drivingModule->moveBackward();
        break;
    case EDGE_FORWARD:
        _drivingModule->moveForward();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_RIGHT:
        _drivingModule->turnRight();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_LEFT:
        _drivingModule->turnLeft();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_REVERSE:
        _drivingModule->moveBackward();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case BEAR_LEFT_FORWARD:
        _drivingModule->bearLeft(DIR_FORWARD);
        break;
    case BEAR_RIGHT_FORWARD:
        _drivingModule->bearRight(DIR_FORWARD);
        break;
    case BEAR_LEFT_REVERSE:
        _drivingModule->bearLeft(DIR_REVERSE);
        break;
    case BEAR_RIGHT_REVERSE:
        _drivingModule->bearRight(DIR_REVERSE);
        break;
    case INCREASE_EDGE_DURATION:
        _edgeModule->increase();
        break;
    case DECREASE_EDGE_DURATION:
        _edgeModule->decrease();
        break;
    case TOGGLE_SELF_DRIVE_MODE:
        toggleSelfDriveMode();
        break;
    case STOP:
        _drivingModule->stop();
        break;
    default:
        if (_awaitingDelayInstruction)
        {
            _awaitingDelayInstruction = false;
            DBGP_PRNT("Waiting for ");
            DBGP_PRNT(instruction);
            DBGP("ms...");
            delay(instruction);
        }
    }
}

void ControlModule::toggleSelfDriveMode()
{
    DBGP("Toggling self-drive mode...");
    turnSelfDriveMode(!_commandModule->selfDriveMode());
}

void ControlModule::turnSelfDriveMode(bool on)
{
    _commandModule->turnSelfDriveMode(on);
    if (_commandModule->selfDriveMode())
    {
        _ledModule->toggleOn(_ledModule->greenPin());
    }
    else
    {
        _ledModule->turnOff(_ledModule->greenPin());
        _drivingModule->stop();
    }
}