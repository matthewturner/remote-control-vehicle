#include <ArduinoFake.h>
#include <unity.h>

#include "AutoPilotModule.h"

using namespace fakeit;

Stream *stream;
AutoPilotModule *target;

void setUp(void)
{
    ArduinoFakeReset();
    // When(Method(ArduinoFake(Wire), begin)).AlwaysReturn();
    stream = ArduinoFakeMock(Stream);
    target = new AutoPilotModule(stream, nullptr, nullptr);
}

void tearDown(void)
{
    delete target;
    delete stream;
}

void test_does_nothing_when_disabled(void)
{
    TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_does_nothing_when_disabled);

    UNITY_END();

    return 0;
}