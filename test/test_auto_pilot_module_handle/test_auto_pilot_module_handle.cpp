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
    result.Back = 10;
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

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_stops_when_trapped);

    UNITY_END();

    return 0;
}