#include <ArduinoFake.h>
#include <unity.h>

#include "AutoPilotModule.h"

using namespace fakeit;

Stream *stream;
AutoPilotModule *target;
Mock<ISensorModule> sensorModuleMock;
Mock<IDrivingModule> drivingModuleMock;

void setUp(void)
{
    ArduinoFakeReset();
    
    stream = ArduinoFakeMock(Stream);
    ISensorModule &sensorModule = sensorModuleMock.get();
    IDrivingModule &drivingModule = drivingModuleMock.get();
    target = new AutoPilotModule(stream, &drivingModule, &sensorModule);
}

void tearDown(void)
{
    delete target;
    delete stream;
}

void test_disabled_by_default(void)
{
    TEST_ASSERT_FALSE(target->enabled());
}

void test_can_be_enabled(void)
{
    target->enable();
    TEST_ASSERT_TRUE(target->enabled());
}

void test_can_be_disabled(void)
{
    target->enable();
    target->disable();
    TEST_ASSERT_FALSE(target->enabled());
}

void test_does_nothing_when_disabled(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    target->handle();
    Verify(Method(ArduinoFake(), millis)).Never();
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_disabled_by_default);
    RUN_TEST(test_can_be_enabled);
    RUN_TEST(test_can_be_disabled);
    RUN_TEST(test_does_nothing_when_disabled);

    UNITY_END();

    return 0;
}