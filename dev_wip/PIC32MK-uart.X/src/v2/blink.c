/*
 * File:   blink.c
 * Author: Matheus Grossi
 */

#include <xc.h>
#include <stdint.h>
#include "aux_func.h"

void blink_led(void)
{
    LATAbits.LATA10 = 1;
    delay_ms(1000);
    LATAbits.LATA10 = 0;
}
