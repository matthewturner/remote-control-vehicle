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
    target->enable();
    When(Method(sensorModuleMock, signalled)).Return(false);

    result.Age = 10;
    result.Front = 10;
    result.Left = 10;
    result.Right = 10;
}

void tearDown(void)
{
    delete target;
    delete stream;
    drivingModuleMock.Reset();
    sensorModuleMock.Reset();
}

void test_stops_when_trapped(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, stop)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, stop)).Once();
}

void test_turns_right_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
    TEST_ASSERT_EQUAL(MAX_SENSOR_AGE_FOR_TURN, target->maxSensorResultAge());
}

void test_turns_left_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnLeft)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, turnLeft)).Once();
    TEST_ASSERT_EQUAL(MAX_SENSOR_AGE_FOR_TURN, target->maxSensorResultAge());
}

void test_turns_right_preferentially_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
    TEST_ASSERT_EQUAL(MAX_SENSOR_AGE_FOR_TURN, target->maxSensorResultAge());
}

void test_turns_right_when_space_ahead_and_no_space_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD - 1;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
    TEST_ASSERT_EQUAL(MAX_SENSOR_AGE_FOR_TURN, target->maxSensorResultAge());
}

void test_turns_left_when_space_ahead_and_no_space_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnLeft)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
    result.Right = SIDE_SENSOR_COLLISION_THRESHOLD - 1;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);

    target->handle();

    Verify(Method(drivingModuleMock, turnLeft)).Once();
    TEST_ASSERT_EQUAL(MAX_SENSOR_AGE_FOR_TURN, target->maxSensorResultAge());
}

void test_bear_right_when_space_ahead_warning_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearRight)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD - 1;
    result.Right = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR * result.Left;

    target->handle();

    Verify(Method(drivingModuleMock, bearRight).Using(DIR_FORWARD)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

void test_bear_left_when_space_ahead_warning_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearLeft)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
    result.Right = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD - 1;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR * result.Right;

    target->handle();

    Verify(Method(drivingModuleMock, bearLeft).Using(DIR_FORWARD)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

void test_move_forward_when_centered(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, moveForward)).AlwaysReturn();

    result.Left = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
    result.Right = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD * result.Front;

    target->handle();

    Verify(Method(drivingModuleMock, moveForward)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

void test_move_forward_when_one_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, moveForward)).AlwaysReturn();

    result.Left = SIDE_SENSOR_CLEAR_THRESHOLD + 2;
    result.Right = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_FORWARD * result.Front;

    target->handle();

    Verify(Method(drivingModuleMock, moveForward)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

void test_bear_right_when_space_ahead_more_space_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearRight)).AlwaysReturn();

    result.Left = SIDE_SENSOR_CLEAR_THRESHOLD - 5;
    result.Right = SIDE_SENSOR_CLEAR_THRESHOLD - 2;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR * result.Left;

    target->handle();

    Verify(Method(drivingModuleMock, bearRight).Using(DIR_FORWARD)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

void test_bear_left_when_space_ahead_more_space_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearLeft)).AlwaysReturn();

    result.Left = SIDE_SENSOR_CLEAR_THRESHOLD - 2;
    result.Right = SIDE_SENSOR_CLEAR_THRESHOLD - 5;
    result.Front = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
    target->updateResult(&result);
    short maxSensorResultAge = MAX_SENSOR_AGE_MULTIPLIER_FOR_BEAR * result.Right;

    target->handle();

    Verify(Method(drivingModuleMock, bearLeft).Using(DIR_FORWARD)).Once();
    TEST_ASSERT_EQUAL(maxSensorResultAge, target->maxSensorResultAge());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_stops_when_trapped);
    RUN_TEST(test_turns_right_when_no_space_ahead);
    RUN_TEST(test_turns_left_when_no_space_ahead);
    RUN_TEST(test_turns_right_preferentially_when_no_space_ahead);
    RUN_TEST(test_turns_right_when_space_ahead_and_no_space_on_left);
    RUN_TEST(test_turns_left_when_space_ahead_and_no_space_on_right);
    RUN_TEST(test_bear_right_when_space_ahead_warning_on_left);
    RUN_TEST(test_bear_left_when_space_ahead_warning_on_right);
    RUN_TEST(test_move_forward_when_centered);
    RUN_TEST(test_move_forward_when_one_side_clear);
    RUN_TEST(test_bear_right_when_space_ahead_more_space_on_right);

    UNITY_END();

    return 0;
}