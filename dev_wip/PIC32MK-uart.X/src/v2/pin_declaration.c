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

    // User LED1 da placa em RA10.
    TRISAbits.TRISA10 = 0;
    LATAbits.LATA10 = 0;
}
