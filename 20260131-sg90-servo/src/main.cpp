#define TEST 2

#if TEST == 1
#include <main1.hpp>
#elif TEST == 2
#include <main2.hpp>
#endif

void setup()
{
#if TEST == 1
    test1::setup();
#elif TEST == 2
    test2::setup();
#endif
}

void loop()
{
#if TEST == 1
    test1::loop();
#elif TEST == 2
    test2::loop();
#endif
}
