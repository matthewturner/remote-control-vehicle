#include <Arduino.h>
#include "CommandModule.h"

CommandModule::CommandModule()
{
}

int CommandModule::tryReadCommand()
{
    int index = -1;
    while (Serial.available())
    {
        delay(2);
        char ch = Serial.read();
        switch (ch)
        {
        case '>':
            index = 0;
            break;
        case '!':
            _commandBuffer[index] = '\0';
            return index;
        default:
            _commandBuffer[index] = ch;
            index++;
            break;
        }
    }
    return -1;
}

int CommandModule::convertToInstruction(int commandLength)
{
    if (strcmp(_commandBuffer, "stop") == 0)
    {
        return STOP;
    }
    if (strcmp(_commandBuffer, "forward") == 0)
    {
        return FORWARD;
    }
    if (strcmp(_commandBuffer, "reverse") == 0)
    {
        return REVERSE;
    }
    if (strcmp(_commandBuffer, "left") == 0)
    {
        return LEFT;
    }
    if (strcmp(_commandBuffer, "right") == 0)
    {
        return RIGHT;
    }
    if (strcmp(_commandBuffer, "bear-left-forward") == 0)
    {
        return BEAR_LEFT_FORWARD;
    }
    if (strcmp(_commandBuffer, "bear-right-forward") == 0)
    {
        return BEAR_RIGHT_FORWARD;
    }
    if (strcmp(_commandBuffer, "bear-left-reverse") == 0)
    {
        return BEAR_LEFT_REVERSE;
    }
    if (strcmp(_commandBuffer, "bear-right-reverse") == 0)
    {
        return BEAR_RIGHT_REVERSE;
    }
    if (strcmp(_commandBuffer, "set-speed-1") == 0)
    {
        return SET_SPEED_1;
    }
    if (strcmp(_commandBuffer, "set-speed-2") == 0)
    {
        return SET_SPEED_2;
    }
    if (strcmp(_commandBuffer, "set-speed-3") == 0)
    {
        return SET_SPEED_3;
    }
    if (strcmp(_commandBuffer, "set-speed-4") == 0)
    {
        return SET_SPEED_4;
    }
    if (strcmp(_commandBuffer, "set-speed-5") == 0)
    {
        return SET_SPEED_5;
    }
    if (strcmp(_commandBuffer, "edge-reverse") == 0)
    {
        return EDGE_REVERSE;
    }
    if (strcmp(_commandBuffer, "edge-left") == 0)
    {
        return EDGE_LEFT;
    }
    if (strcmp(_commandBuffer, "edge-right") == 0)
    {
        return EDGE_RIGHT;
    }
    if (strcmp(_commandBuffer, "edge-forward") == 0)
    {
        return EDGE_FORWARD;
    }
    if (strcmp(_commandBuffer, "start-self-drive") == 0)
    {
        if (selfDriveMode)
        {
            return -1;
        }
        else
        {
            return TOGGLE_SELF_DRIVE_MODE;
        }
    }
    if (strcmp(_commandBuffer, "stop-self-drive") == 0)
    {
        if (selfDriveMode)
        {
            return TOGGLE_SELF_DRIVE_MODE;
        }
        else
        {
            return -1;
        }
    }
    if (strcmp(_commandBuffer, "replay") == 0)
    {
        return REPLAY;
    }
    if (strcmp(_commandBuffer, "increase-edge-duration") == 0)
    {
        return INCREASE_EDGE_DURATION;
    }
    if (strcmp(_commandBuffer, "decrease-edge-duration") == 0)
    {
        return DECREASE_EDGE_DURATION;
    }
    if (strcmp(_commandBuffer, "start-record") == 0)
    {
        if (recordMode)
        {
            return -1;
        }
        else
        {
            return TOGGLE_RECORD_MODE;
        }
    }
    if (strcmp(_commandBuffer, "stop-record") == 0)
    {
        if (recordMode)
        {
            return TOGGLE_RECORD_MODE;
        }
        else
        {
            return -1;
        }
    }
    if (strcmp(_commandBuffer, "start-edge") == 0)
    {
        if (edgeMode)
        {
            return -1;
        }
        else
        {
            return TOGGLE_EDGE_MODE;
        }
    }
    if (strcmp(_commandBuffer, "stop-edge") == 0)
    {
        if (edgeMode)
        {
            return TOGGLE_EDGE_MODE;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}