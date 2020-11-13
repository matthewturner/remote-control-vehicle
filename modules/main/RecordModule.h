#ifndef RecordModule_h
#define RecordModule_h

class RecordModule
{
public:
    RecordModule(short replayDelay);

    void recordInstruction(int instruction);
    void recordDelay();
    void setTimeOfLastInstruction(unsigned long time);
    unsigned int timeSinceLastInstruction(unsigned long time);

    int next();
    int peek();
    short replay();
    void reset();

private:
    int _currentIndex = -1;
    int _replayIndex = -1;
    int _instructions[300];
    short _replayDelay;
    unsigned long _timeOfLastInstruction;
};

#endif