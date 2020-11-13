#include <Arduino.h>
#include "RecordModule.h"
#include "Debug.h"

RecordModule::RecordModule(short replayDelay)
{
    _replayDelay = replayDelay;
}

void RecordModule::setTimeOfLastInstruction(unsigned long time)
{
    _timeOfLastInstruction = time;
}

unsigned int RecordModule::timeSinceLastInstruction(unsigned long time)
{
    return time - _timeOfLastInstruction;
}

int RecordModule::next()
{
    _replayIndex++;
    if (_replayIndex >= _currentIndex)
    {
        return -1;
    }
    return _instructions[_replayIndex];
}

int RecordModule::peek()
{
    return _instructions[_replayIndex + 1];
}

short RecordModule::replay()
{
    _replayIndex = -1;
    return _currentIndex;
}

void RecordModule::reset()
{
    _replayIndex = -1;
    _currentIndex = -1;
}

void RecordModule::recordInstruction(int instruction)
{
    if (instruction == 0)
    {
        if (_currentIndex > 0)
        {
            if (_instructions[_currentIndex - 1] == 0)
            {
                return;
            }
        }
    }
    DBG_PRNT("Recording instruction ");
    DBG_PRNT(instruction, HEX);
    DBG("...");
    _instructions[_currentIndex] = instruction;
    _currentIndex++;
}
