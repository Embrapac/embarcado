/*
 * File:   pins.c
 * Author: Matheus Grossi
 */

#include "defs.h"
#include "pins.h"

#include <xc.h>

void pin_defs(void)
{
    // Garante JTAG desligado também em runtime.
    CFGCONbits.JTAGEN = 0;

    // Estado inicial do LED em RB10: desligado.
    RB10_state = 0;

    // RB10 como saída.
    RB10_dir = 0;
}

void button_def(void)
{
    // RB4 como entrada: botão liga.
    RB1_dir = 1;
    RB1_state = 0;

    // RB11 como entrada: botão desliga.
    RB2_dir = 1;
    RB2_state = 0;
}
