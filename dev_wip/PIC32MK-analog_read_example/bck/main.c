#define _CONFIG_BITS_SOURCE
#include "defs.h"
#include "pins.h"
#include "adc_1.h"

int main(void)
{
    init_pins();
    init_OSC();
    init_TMR2();
    init_ADC();

    INTCONSET = _INTCON_MVEC_MASK;
    __builtin_enable_interrupts();

    while (1)
    {
        if (g_adc0_new_sample != 0u)
        {
            g_adc0_new_sample = 0u;
            analog_process_sample();
        }
    }

    return 0;
}
