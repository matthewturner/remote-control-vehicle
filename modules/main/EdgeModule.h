#ifndef EdgeModule_h
#define EdgeModule_h

#define DEFAULT_DURATION 200

class EdgeModule
{
public:
    EdgeModule();
    void increase();
    void decrease();
    void reset();
    void wait();

private:
    short _duration;
};

#endif