#include "pins.h"
#include "adc_1.h"

#include "on_off_button.h"
#include "out_control.h"

void init(void)
{
    CFGCONbits.JTAGEN = 0;

    /* Inicializa todos os pinos como digitais */
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;

    /* AN0 no MCA048 está em RA0 */
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;

    /* Entradas */
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    /* Saídas do primeiro conjunto */
    TRISBbits.TRISB4  = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB13 = 0;

    /* Saídas do segundo conjunto */
    RLED_DIR = 0;
    GLED_DIR = 0;
    YLED_DIR = 0;

    /* Garantia explícita de entradas digitais */
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;

    on_off_button_init();
    LEDs_ClearAll();

    init_OSC();
    init_TMR2();
    init_ADC();

    /* Sistema de medição inicia desabilitado */
    T2CONbits.ON = 0;

    INTCONSET = _INTCON_MVEC_MASK;
    __builtin_enable_interrupts();
}
