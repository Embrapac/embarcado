#include "defs.h"

#include "init.h"

#define CPU_FREQ_HZ        4000000UL   /* Ajuste para o clock real da CPU */
#define CORE_TIMER_HZ      (CPU_FREQ_HZ / 2UL)

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

void delay_ms(uint32_t ms)
{
    const uint32_t ticks_per_ms = CORE_TIMER_HZ / 1000UL;

    while (ms-- != 0u)
    {
        _CP0_SET_COUNT(0);

        while (_CP0_GET_COUNT() < ticks_per_ms)
        {
        }
    }
}