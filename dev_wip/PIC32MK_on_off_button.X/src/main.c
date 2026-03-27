/* 
 * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

#define _CONFIG_BITS_SOURCE

#include "defs.h"

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

int state = 0;

void main()
{
    //CMCON = 0x07;
    CFGCONbits.JTAGEN = 0;

    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB10 = 0;
    
    PORTBbits.RB10 = 0;

    while(1){
        if ( (PORTBbits.RB1) && (!PORTBbits.RB2) ) { // Botao pressionado
            delay_ms(500);
            PORTBbits.RB10 = 1;
        }
        else
        if ( (!PORTBbits.RB1) && (PORTBbits.RB2) ) { // Botao solto
            delay_ms(500);
            PORTBbits.RB10 = 0;
        }
    }
}