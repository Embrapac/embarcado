#define _CONFIG_BITS_SOURCE
#include "defs.h"
#include "adc_1.h"
#include "pins.h"

void init_OSC(void)
{
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    /* PBCLK2 = SYSCLK / 1 */
    PB2DIVbits.PBDIV = 0;

    SYSKEY = 0x33333333;
}

void init_TMR2(void)
{
    PR2 = 250000u - 16u;

    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 4;
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 1;

    IPC2bits.T2IP = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;

    T2CONbits.ON = 1;
}

int main(void)
{
    Pins_Init();
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
