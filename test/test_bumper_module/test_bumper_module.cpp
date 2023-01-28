#include <ArduinoFake.h>
#include <unity.h>

#include "BumperModule.h"

using namespace fakeit;

const byte LEFT_PIN = 2;
const byte RIGHT_PIN = 3;

Stream *stream;
BumperModule *target;

void setUp(void)
{
    ArduinoFakeReset();

    stream = ArduinoFakeMock(Stream);
    target = new BumperModule(LEFT_PIN, RIGHT_PIN, stream);
}

void tearDown(void)
{
    delete target;
    delete stream;
}

void test_has_not_collided(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(HIGH);
    TEST_ASSERT_EQUAL(Sides::NONE, target->hasCollided());
}

void test_has_collided_on_left(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(HIGH);
    TEST_ASSERT_EQUAL(Sides::LEFT, target->hasCollided());
}

void test_has_collided_on_right(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW);
    TEST_ASSERT_EQUAL(Sides::RIGHT, target->hasCollided());
}

void test_has_collided_on_both(void)
{
    When(Method(ArduinoFake(), millis)).Return(1);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW);
    TEST_ASSERT_EQUAL(Sides::BOTH, target->hasCollided());
}

void test_has_not_collided_on_left_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(HIGH, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::NONE, target->hasCollided(8));
}

void test_has_not_collided_on_right_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(HIGH, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::NONE, target->hasCollided(8));
}

void test_has_not_collided_on_both_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::NONE, target->hasCollided(8));
}

void test_has_collided_on_left_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(HIGH, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::LEFT, target->hasCollided(9));
}

void test_has_collided_on_right_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(HIGH, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::RIGHT, target->hasCollided(9));
}

void test_has_collided_on_both_recently(void)
{
    When(Method(ArduinoFake(), millis)).Return(1, 10);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(LOW, HIGH);
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::BOTH, target->hasCollided(9));
}

void test_has_collided_on_both_recently_but_different_time(void)
{
    When(Method(ArduinoFake(), millis)).Return(9, 11, 15);
    When(Method(ArduinoFake(), digitalRead).Using(LEFT_PIN)).Return(LOW, HIGH, HIGH);
    When(Method(ArduinoFake(), digitalRead).Using(RIGHT_PIN)).Return(HIGH, LOW, HIGH);
    target->hasCollided();
    target->hasCollided();
    TEST_ASSERT_EQUAL(Sides::BOTH, target->hasCollided(9));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_has_not_collided);
    RUN_TEST(test_has_collided_on_left);
    RUN_TEST(test_has_collided_on_right);
    RUN_TEST(test_has_collided_on_both);
    RUN_TEST(test_has_not_collided_on_left_recently);
    RUN_TEST(test_has_not_collided_on_right_recently);
    RUN_TEST(test_has_not_collided_on_both_recently);
    RUN_TEST(test_has_collided_on_left_recently);
    RUN_TEST(test_has_collided_on_right_recently);
    RUN_TEST(test_has_collided_on_both_recently);
    RUN_TEST(test_has_collided_on_both_recently_but_different_time);

    UNITY_END();

    return 0;
}