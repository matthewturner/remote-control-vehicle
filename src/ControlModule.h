#ifndef ControlModule_h
#define ControlModule_h

#include "DrivingModule.h"
#include "LedModule.h"
#include "EdgeModule.h"
#include "CommandModule.h"

class ControlModule
{
public:
    ControlModule(HardwareSerial *stream, IDrivingModule *drivingModule,
                  EdgeModule *edgeModule,
                  LedModule *ledModule, CommandModule *commandModule);

    void executeInstruction(int instruction);

private:
    HardwareSerial *_stream;
    IDrivingModule *_drivingModule;
    EdgeModule *_edgeModule;
    LedModule *_ledModule;
    CommandModule *_commandModule;
    bool _awaitingDelayInstruction;

    void toggleSelfDriveMode();
    void turnSelfDriveMode(bool on);
};

#endif