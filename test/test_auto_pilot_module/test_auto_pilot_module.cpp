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

    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isTrapped());
}

void test_is_not_trapped_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Right.Distance = 11;

    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_is_not_trapped_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Left.Distance = 11;

    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isTrapped());
}

void test_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    result.Front.Distance = 11;

    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->spaceAhead());
}

void test_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);

    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->spaceAhead());
}

void test_result_not_requested(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(sensorModuleMock, signalled)).Return(false);

    When(Method(drivingModuleMock, stop)).AlwaysReturn();

    target->updateResult(&result);

    target->updatePositionIfRequired();

    TEST_ASSERT_EQUAL(10, target->sensorResult()->Front.Distance);
    TEST_ASSERT_EQUAL(10, target->sensorResult()->Front.Timestamp);
    Verify(Method(drivingModuleMock, stop)).Never();
}

void test_result_requested_if_signalled(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 11, 12);

    When(Method(sensorModuleMock, signalled)).Return(true);
    When(Method(sensorModuleMock, detect))
        .Do([](SensorResult* r)->bool
        {
            r->Front.Distance = 6;
            r->Front.Timestamp = 0;
            return true;
        });

    When(Method(drivingModuleMock, stop)).AlwaysReturn();

    target->updateResult(&result);
    target->updatePositionIfRequired();

    TEST_ASSERT_EQUAL(0, target->sensorResult()->Front.Timestamp);
    TEST_ASSERT_EQUAL(6, target->sensorResult()->Front.Distance);
    Verify(Method(drivingModuleMock, stop)).Once();
}

void test_result_requested_if_result_too_old(void)
{
    When(Method(ArduinoFake(), millis)).Return(4000, 4001, 4001);

    When(Method(sensorModuleMock, signalled)).Return(false);
    When(Method(sensorModuleMock, detect))
        .Do([](SensorResult* r)->bool
        {
            r->Front.Distance = 6;
            r->Front.Timestamp = 20;
            return true;
        });

    When(Method(drivingModuleMock, stop)).AlwaysReturn();

    target->updateResult(&result);
    target->updatePositionIfRequired();

    TEST_ASSERT_EQUAL(20, target->sensorResult()->Front.Timestamp);
    TEST_ASSERT_EQUAL(6, target->sensorResult()->Front.Distance);
    Verify(Method(drivingModuleMock, stop)).Once();
}

void test_neither_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isOneSideClear());
}

void test_left_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD + 1;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isOneSideClear());
}

void test_right_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD;
    result.Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD + 1;
    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isOneSideClear());
}

void test_centered(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 50;
    result.Right.Distance = 50;
    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_centered_within_right_tolerance(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 50;
    result.Right.Distance = 49;
    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_centered_within_left_tolerance(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 49;
    result.Right.Distance = 50;
    target->updateResult(&result);

    TEST_ASSERT_TRUE(target->isCentered());
}

void test_not_centered_towards_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 50;
    result.Right.Distance = 48;
    target->updateResult(&result);

    TEST_ASSERT_FALSE(target->isCentered());
}

void test_not_centered_towards_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10);

    result.Left.Distance = 48;
    result.Right.Distance = 50;
    target->updateResult(&result);

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
    RUN_TEST(test_result_not_requested);
    RUN_TEST(test_result_requested_if_signalled);
    RUN_TEST(test_result_requested_if_result_too_old);
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