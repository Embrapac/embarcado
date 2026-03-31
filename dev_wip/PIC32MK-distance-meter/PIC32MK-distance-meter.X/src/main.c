#define _CONFIG_BITS_SOURCE
#include "defs.h"
#include "adc_1.h"
#include "pins.h"
#include "on_off_button.h"
#include "init.h"

int main(void)
{
    init();

    while (1)
    {
        operation();

        if (g_adc0_new_sample != 0u)
        {
            g_adc0_new_sample = 0u;
            analog_process_sample();
        }
    }

    return 0;
}