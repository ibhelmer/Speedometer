#include "delay.h"
void DelayMs(uint16_t  milliseconds)
{
    uint32_t time;
    time = READ_CORE_TIMER(); // Read Core Timer
    time += (MY_SYS_CLK_FREQ / 2 / 1000) * milliseconds; // calc the Stop Time
    while ((int32_t)(time - READ_CORE_TIMER()) > 0){};
}
void DelayUs(uint16_t microseconds)
{
uint32_t time;
    time = READ_CORE_TIMER(); // Read Core Timer
    time += (MY_SYS_CLK_FREQ / 2 / 1000000) * microseconds; // calc the Stop Time
    while ((int32_t)(time - READ_CORE_TIMER()) > 0){};
}
void DelayNs(uint16_t nanoseconds)
{
uint32_t time;
    time = READ_CORE_TIMER(); // Read Core Timer
    time += (MY_SYS_CLK_FREQ / 2 / 1000000000) * nanoseconds; // calc the Stop Time
    while ((int32_t)(time - READ_CORE_TIMER()) > 0){};
}
