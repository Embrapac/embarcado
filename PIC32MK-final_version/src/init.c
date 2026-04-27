#include "defs.h"
#include "init.h"
#include "uart.h"

void init_OSC(void)
{
    /*
     * O projeto consolidado usa a mesma base de clock definida em defs.h.
     * A UART e o timer de amostragem compartilham essa referencia.
     */
}

void init_TMR2(void)
{
    const uint32_t pr_value =
        ((PBCLK_HZ / TMR2_PRESCALE_VALUE) * MEASUREMENT_PERIOD_MS / 1000UL) - 1UL;

    T2CONbits.ON = 0;
    T2CONbits.TCKPS = TMR2_PRESCALE_BITS;
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 0;

    TMR2 = 0u;
    PR2 = (uint16_t)pr_value;

    IPC2bits.T2IP = 1;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void delay_ms(uint32_t ms)
{
    const uint32_t ticks_per_ms = CORE_TIMER_HZ / 1000UL;

    while (ms-- != 0u)
    {
        const uint32_t start = (uint32_t)_CP0_GET_COUNT();

        while (((uint32_t)_CP0_GET_COUNT() - start) < ticks_per_ms)
        {
        }

        uart1_led_service_1ms();
    }
}
