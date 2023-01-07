#include <ArduinoFake.h>
#include <unity.h>

#include "AutoPilotModule.h"

using namespace fakeit;

Stream *stream;
AutoPilotModule *target;
Mock<ISensorModule> sensorModuleMock;
Mock<IDrivingModule> drivingModuleMock;
SensorResult result;

void setUp(void)
{
    ArduinoFakeReset();
    
    stream = ArduinoFakeMock(Stream);
    ISensorModule &sensorModule = sensorModuleMock.get();
    IDrivingModule &drivingModule = drivingModuleMock.get();
    target = new AutoPilotModule(stream, &drivingModule, &sensorModule);

    result.Age = 10;
    result.Front = 10;
    result.Back = 10;
    result.Left = 10;
    result.Right = 10;
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

void test_is_trapped(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isTrapped());
}

void test_is_not_trapped_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Right = 11;

    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_is_not_trapped_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Left = 11;

    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Front = 11;

    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->spaceAhead());
}

void test_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    
    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->spaceAhead());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_disabled_by_default);
    RUN_TEST(test_can_be_enabled);
    RUN_TEST(test_can_be_disabled);
    RUN_TEST(test_does_nothing_when_disabled);
    RUN_TEST(test_is_trapped);
    RUN_TEST(test_is_not_trapped_on_right);
    RUN_TEST(test_is_not_trapped_on_left);
    RUN_TEST(test_space_ahead);
    RUN_TEST(test_no_space_ahead);

    UNITY_END();

    return 0;
}