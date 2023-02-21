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
    target = new AutoPilotModule(stream, &drivingModule, &sensorModule, &result);

    result.Front.Distance = 10;
    result.Front.Timestamp = 10;
    result.Left.Distance = 10;
    result.Left.Timestamp = 10;
    result.Right.Distance = 10;
    result.Right.Timestamp = 10;
}

void tearDown(void)
{
    delete target;
    delete stream;
    drivingModuleMock.Reset();
    sensorModuleMock.Reset();
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

    TEST_ASSERT_TRUE(target->isTrapped());
}

void test_is_not_trapped_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_is_not_trapped_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Front.Distance = 101;

    TEST_ASSERT_TRUE(target->spaceAhead());
}

void test_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    TEST_ASSERT_FALSE(target->spaceAhead());
}

void test_neither_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;

    TEST_ASSERT_FALSE(target->isOneSideClear());
}

void test_left_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD + 1;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;

    TEST_ASSERT_TRUE(target->isOneSideClear());
}

void test_right_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD + 1;

    TEST_ASSERT_TRUE(target->isOneSideClear());
}

void test_centered(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 50;
    result.Right.Distance = 50;

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_centered_within_right_tolerance(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 50;
    result.Right.Distance = 49;

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_centered_within_left_tolerance(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 49;
    result.Right.Distance = 50;

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_not_centered_towards_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 500;
    result.Right.Distance = 480;

    TEST_ASSERT_FALSE(target->isCentered());
}

void test_not_centered_towards_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 480;
    result.Right.Distance = 500;

    TEST_ASSERT_FALSE(target->isCentered());
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
    RUN_TEST(test_neither_side_clear);
    RUN_TEST(test_left_side_clear);
    RUN_TEST(test_right_side_clear);
    RUN_TEST(test_centered);
    RUN_TEST(test_centered_within_right_tolerance);
    RUN_TEST(test_centered_within_left_tolerance);
    RUN_TEST(test_not_centered_towards_right);
    RUN_TEST(test_not_centered_towards_left);

    UNITY_END();

    return 0;
}