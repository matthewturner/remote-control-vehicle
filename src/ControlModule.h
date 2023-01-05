#ifndef ControlModule_h
#define ControlModule_h

#include "DrivingModule.h"
#include "LedModule.h"
#include "EdgeModule.h"
#include "RecordModule.h"
#include "CommandModule.h"

class ControlModule
{
public:
    ControlModule(HardwareSerial *stream, DrivingModule *drivingModule,
                  RecordModule *recordModule, EdgeModule *edgeModule,
                  LedModule *ledModule, CommandModule *commandModule);

    void executeInstruction(int instruction);

private:
    HardwareSerial *_stream;
    DrivingModule *_drivingModule;
    RecordModule *_recordModule;
    EdgeModule *_edgeModule;
    LedModule *_ledModule;
    CommandModule *_commandModule;
    bool _awaitingDelayInstruction;

    void recordDurationIfRequired();
    void replayInstructions();
    void toggleSelfDriveMode();
    void turnSelfDriveMode(bool on);
    void toggleRecordMode();
    void turnRecordMode(bool on);
    void recordInstructionIfRequired(int instruction);
};

#endif