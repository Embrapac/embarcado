/*
 * File:   pin_declaration.c
 * Author: Matheus Grossi
 * Brief:  Definicoes de inicializacao de pinos.
 */

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

void pins_init(void)
{
    // Garante JTAG desligado em runtime.
    CFGCONbits.JTAGEN = 0;

    // LED de transmissao em RB14.
    //ANSELBbits.ANSB14 = 0;
    TRISBbits.TRISB14 = 0;
    LATBbits.LATB14 = 0;

    // LED de recepcao em RB15.
    //ANSELBbits.ANSB15 = 0;
    TRISBbits.TRISB15 = 0;
    LATBbits.LATB15 = 0;
}
