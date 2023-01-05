#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;


void setUp(void)
{
    ArduinoFakeReset();

}

void tearDown(void)
{
}

void test_something(void)
{
    TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_something);

    UNITY_END();

    return 0;
}