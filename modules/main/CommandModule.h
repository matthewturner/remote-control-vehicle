#ifndef CommandModule_h
#define CommandModule_h

const int LEFT = 0xFF22DD;
const int RIGHT = 0xFFC23D;
const int FORWARD = 0xFF906F;
const int REVERSE = 0xFFE01F;
const int EDGE_LEFT = 0xFF22DC;
const int EDGE_RIGHT = 0xFFC23C;
const int EDGE_FORWARD = 0xFF906E;
const int EDGE_REVERSE = 0xFFE01E;
const int BEAR_LEFT_FORWARD = 0xFF22DB;
const int BEAR_RIGHT_FORWARD = 0xFFC23B;
const int BEAR_LEFT_REVERSE = 0xFF906D;
const int BEAR_RIGHT_REVERSE = 0xFFE01D;
const int STOP = 0;
const int REPLAY = 0xFF6897;
const int INCREASE_EDGE_DURATION = 0xFF629D;
const int DECREASE_EDGE_DURATION = 0xFFA857;
const int TOGGLE_RECORD_MODE = 0xFFB04F;
const int TOGGLE_EDGE_MODE = 0xFF9867;
const int TOGGLE_SELF_DRIVE_MODE = 0xFF9868;
const int DELAY = 0xAAAAAA;
const int PLAY_PAUSE = 0xFF02FD;
const int SET_SPEED_1 = 0xAAAAAB;
const int SET_SPEED_2 = 0xAAAAAC;
const int SET_SPEED_3 = 0xAAAAAD;
const int SET_SPEED_4 = 0xAAAAAE;
const int SET_SPEED_5 = 0xAAAAAF;

class CommandModule
{
public:
    CommandModule(HardwareSerial *stream);
    int tryReadCommand();
    int tryReadInstruction();
    int convertToInstruction(int commandLength);
    void turnRecordMode(bool on);
    void turnSelfDriveMode(bool on);

    bool recordMode();
    bool selfDriveMode();

private:
    char _commandBuffer[30];
    bool _selfDriveMode;
    bool _recordMode;
    HardwareSerial *_stream;
};

#endif