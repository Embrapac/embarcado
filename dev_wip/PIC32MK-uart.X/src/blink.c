/* 
 * File:   blink.c
 * Author: Matheus Grossi
 */

#include <xc.h>
#include <stdint.h>
#include "aux_func.h"

void blink_led(void)
{
    LATBbits.LATB10 = 1;   // acende
    delay_ms(1000);
    LATBbits.LATB10 = 0;   // apaga
}
