/* 
 * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

#define _CONFIG_BITS_SOURCE

#include "defs.h"
#include "pins.h"

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

int state = 0;

/*
void button_operation(void)
{
    if ( (RB4_read) && (!RB11_read) ) { // Botao pressionado
        delay_ms(500);
        RB10_state = 1;
    }
    if ( (!RB4_read) && (RB11_read) ) { // Botao solto
        delay_ms(500);
        RB10_state = 0;
    }
}
*/

int main(void)
{

    pin_defs();

    while(1){
        if ( (RB4_read) && (!RB11_read) ) { // Botao pressionado
            delay_ms(500);
            RB10_state = 1;
        }
        if ( (!RB4_read) && (RB11_read) ) { // Botao solto
            delay_ms(500);
            RB10_state = 0;
        }
    }
}
