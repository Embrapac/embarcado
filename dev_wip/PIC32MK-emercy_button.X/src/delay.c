#include "defs.h"

#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>

void delay_ms(uint32_t ms)
{
    // CP0 Count geralmente incrementa a SYSCLK/2
    const uint32_t ticks_per_ms = (SYSCLK_HZ / 2U) / 1000U;
    uint32_t start = _CP0_GET_COUNT();
    uint32_t wait  = ticks_per_ms * ms;

    while ((uint32_t)(_CP0_GET_COUNT() - start) < wait) { }
}
