#ifndef EdgeModule_h
#define EdgeModule_h

#define DEFAULT_DURATION 200

class EdgeModule
{
public:
    EdgeModule(HardwareSerial *stream);
    void increase();
    void decrease();
    void reset();
    short duration();

private:
    short _duration;
    HardwareSerial *_stream;
};

#endif