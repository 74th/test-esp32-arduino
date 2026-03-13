#if TEST_NO == 1
#include <main1.hpp>
#elif TEST_NO == 2
#include <main2.hpp>
#elif TEST_NO == 3
#include <main3.hpp>
#elif TEST_NO == 4
#include <main4.hpp>
#endif

void setup()
{
#if TEST_NO == 1
    test1::setup();
#elif TEST_NO == 2
    test2::setup();
#elif TEST_NO == 3
    test3::setup();
#elif TEST_NO == 4
    test4::setup();
#endif
}

void loop()
{
#if TEST_NO == 1
    test1::loop();
#elif TEST_NO == 2
    test2::loop();
#elif TEST_NO == 3
    test3::loop();
#elif TEST_NO == 4
    test4::loop();
#endif
}
