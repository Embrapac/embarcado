/* 
 * File:   pins.c
 * Author: Matheus Grossi
 */

#include "defs.h"
#include "pins.h"

#include <xc.h>

void pin_defs(void)
{
    // garante JTAG desligado em runtime também
    CFGCONbits.JTAGEN = 0;

    //-----------------------------------------------------
    // Estado inicial de RB10 - Low:
    RB10_state = 0;

    // RB10 como saída:
    RB10_dir = 0;
    //-----------------------------------------------------

    //-----------------------------------------------------
    // RB4 como entrada:
    RB4_dir = 1;

    // RB11 como entrada:
    RB11_dir = 1;
    //-----------------------------------------------------
}


