#include <Arduino.h>
#include "ControlModule.h"
#include "Debug.h"

ControlModule::ControlModule(HardwareSerial *stream, DrivingModule *drivingModule,
                             RecordModule *recordModule, EdgeModule *edgeModule,
                             LedModule *ledModule, CommandModule *commandModule)
{
    _stream = stream;
    _recordModule = recordModule;
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
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->setSpeed(1);
        break;
    case SET_SPEED_2:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->setSpeed(2);
        break;
    case SET_SPEED_3:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->setSpeed(3);
        break;
    case SET_SPEED_4:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->setSpeed(4);
        break;
    case SET_SPEED_5:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->setSpeed(5);
        break;
    case FORWARD:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->moveForward();
        break;
    case RIGHT:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->turnRight();
        break;
    case LEFT:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->turnLeft();
        break;
    case REVERSE:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->moveBackward();
        break;
    case EDGE_FORWARD:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->moveForward();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_RIGHT:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->turnRight();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_LEFT:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->turnLeft();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case EDGE_REVERSE:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->moveBackward();
        delay(_edgeModule->duration());
        _drivingModule->stop();
        break;
    case BEAR_LEFT_FORWARD:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->bearLeft(DIR_FORWARD);
        break;
    case BEAR_RIGHT_FORWARD:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->bearRight(DIR_FORWARD);
        break;
    case BEAR_LEFT_REVERSE:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->bearLeft(DIR_REVERSE);
        break;
    case BEAR_RIGHT_REVERSE:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
        _drivingModule->bearRight(DIR_REVERSE);
        break;
    case INCREASE_EDGE_DURATION:
        recordInstructionIfRequired(instruction);
        _edgeModule->increase();
        break;
    case DECREASE_EDGE_DURATION:
        recordInstructionIfRequired(instruction);
        _edgeModule->decrease();
        break;
    case TOGGLE_RECORD_MODE:
        toggleRecordMode();
        break;
    case TOGGLE_SELF_DRIVE_MODE:
        toggleSelfDriveMode();
        break;
    case REPLAY:
        replayInstructions();
        break;
    case STOP:
        recordDurationIfRequired();
        recordInstructionIfRequired(instruction);
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

void ControlModule::recordDurationIfRequired()
{
    if (!_commandModule->recordMode())
    {
        return;
    }

    unsigned long time = millis();
    if (_drivingModule->isMoving())
    {
        int duration = _recordModule->timeSinceLastInstruction(time);
        _recordModule->recordInstruction(0xAAAAAA);
        _recordModule->recordInstruction(duration);
    }
    _recordModule->setTimeOfLastInstruction(time);
}

void ControlModule::replayInstructions()
{
    _edgeModule->reset();
    DBGP("Replaying instructions...");
    _recordModule->replay();
    int instruction;
    while (instruction = _recordModule->next() != -1)
    {
        executeInstruction(instruction);
        if (instruction != DELAY && _recordModule->peek() != DELAY)
        {
            delay(REPLAY_DELAY);
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
    }
}

void ControlModule::toggleRecordMode()
{
    DBGP("Toggling record mode...");
    turnRecordMode(!_commandModule->recordMode());
}

void ControlModule::turnRecordMode(bool on)
{
    _commandModule->turnRecordMode(on);
    if (_commandModule->recordMode())
    {
        _ledModule->toggleOn(_ledModule->redPin());
        _recordModule->reset();
        _edgeModule->reset();
        _drivingModule->resetSpeed();
    }
    else
    {
        _ledModule->turnOff(_ledModule->redPin());
        _edgeModule->reset();
        _drivingModule->resetSpeed();
    }
}

void ControlModule::recordInstructionIfRequired(int instruction)
{
    if (_commandModule->recordMode())
    {
        _recordModule->recordInstruction(instruction);
    }
}