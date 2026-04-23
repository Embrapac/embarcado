/*
 * File:   blink.c
 * Author: Matheus Grossi
 */

#include <xc.h>
#include <stdint.h>
#include "aux_func.h"
#include "uart.h"

void blink_led(void)
{
    uint32_t i;

    LATBbits.LATB14 = 1;

    for (i = 0u; i < 1000u; i++)
    {
        delay_ms(1);
        uart1_rx_poll();
        uart1_rx_led_service_1ms();
    }

    LATBbits.LATB14 = 0;
}
