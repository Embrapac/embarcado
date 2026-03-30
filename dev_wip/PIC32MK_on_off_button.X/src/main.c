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

int init(void){

    CFGCONbits.JTAGEN = 0;

    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB13 = 0;
    
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;

    //RB10 e RB13 nao tem modo analogico
    //ANSELBbits.ANSB10 = 0;
    //ANSELBbits.ANSB13 = 0;
    
    LATBbits.LATB10 = 0;
    LATBbits.LATB13 = 1;
}

int main(void)
{
    init();
    
    while(1){
        if ( PORTBbits.RB1 ) { // Opera��o de liga
            delay_ms(500);
            LATBbits.LATB10 = 0;
            LATBbits.LATB13 = 1;
        }
        else
        if ( PORTBbits.RB2 ) { // Opera��o de desliga
            delay_ms(500);
            LATBbits.LATB10 = 1;
            LATBbits.LATB13 = 0;
        }
    }
}