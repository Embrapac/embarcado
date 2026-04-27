/* 
 * File:   pin_declaration.c
 * Author: Matheus Grossi
 * Brief:  Defini��es de inicializa��o de pinos.
 */

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

void pins_init(void)
{
    // garante JTAG desligado em runtime tamb�m
    CFGCONbits.JTAGEN = 0;

    //-----------------------------------------------------
    // RB10 como saida:
    TRISBbits.TRISB10 = 0;

    // Estado inicial de RB10 - Low:
    LATBbits.LATB10 = 0;
    //-----------------------------------------------------
}