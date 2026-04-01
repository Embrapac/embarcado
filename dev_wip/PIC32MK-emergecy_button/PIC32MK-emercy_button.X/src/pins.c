#include "pins.h"

#include "on_off_button.h"

void init(void)
{
    CFGCONbits.JTAGEN = 0;

    // Entradas
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    // Saídas
    TRISBbits.TRISB4  = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB13 = 0;

    // Entradas digitais
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;

    on_off_button_init();
}
