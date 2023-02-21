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
    target->enable();
    When(Method(sensorModuleMock, signalled)).Return(false);

    result.Front.Distance = 100;
    result.Front.Timestamp = 10;
    result.Left.Distance = 100;
    result.Left.Timestamp = 10;
    result.Right.Distance = 100;
    result.Right.Timestamp = 10;
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

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Front.Distance = 60;
            r->Front.Timestamp = 20;
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, stop)).Once();
}

void test_turns_right_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
}

void test_turns_left_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnLeft)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, turnLeft)).Once();
}

void test_turns_right_preferentially_when_no_space_ahead(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
}

void test_turns_right_when_space_ahead_and_no_space_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnRight)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD - 1;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, turnRight)).Once();
}

void test_turns_left_when_space_ahead_and_no_space_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, turnLeft)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_THRESHOLD + 1;
            r->Right.Distance = SIDE_SENSOR_COLLISION_THRESHOLD - 1;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, turnLeft)).Once();
}

void test_bear_right_when_space_ahead_warning_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearRight)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD - 1;
            r->Right.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, bearRight).Using(DIR_FORWARD)).Once();
}

void test_bear_left_when_space_ahead_warning_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearLeft)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
            r->Right.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD - 1;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, bearLeft).Using(DIR_FORWARD)).Once();
}

void test_move_forward_when_centered(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, moveForward)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
            r->Right.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, moveForward)).Once();
}

void test_move_forward_when_one_side_clear(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, moveForward)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD + 2;
            r->Right.Distance = SIDE_SENSOR_COLLISION_WARNING_THRESHOLD + 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, moveForward)).Once();
}

void test_bear_right_when_space_ahead_more_space_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearRight)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD - 5;
            r->Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD - 2;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, bearRight).Using(DIR_FORWARD)).Once();
}

void test_bear_left_when_space_ahead_more_space_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(10, 30, 35);

    When(Method(drivingModuleMock, bearLeft)).AlwaysReturn();

    When(Method(sensorModuleMock, scan))
        .Do([](SensorResult *r) -> bool
            {
            r->Left.Distance = SIDE_SENSOR_CLEAR_THRESHOLD - 2;
            r->Right.Distance = SIDE_SENSOR_CLEAR_THRESHOLD - 5;
            r->Front.Distance = FRONT_SENSOR_COLLISION_THRESHOLD + 1;
            return true; });

    target->handle();

    Verify(Method(drivingModuleMock, bearLeft).Using(DIR_FORWARD)).Once();
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