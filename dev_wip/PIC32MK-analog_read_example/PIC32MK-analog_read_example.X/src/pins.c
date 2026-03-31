#include "pins.h"
#include "out_control.h"

void Pins_Init(void)
{
    /*
     * AN0 no MCA048 está em RA0.
     * LEDs externos: RB12 (verde), RB9 (vermelho) e RB3 (amarelo).
     * RB1 e RB11 serão usados como botões digitais.
     */
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;

    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;

    //START_BUTTON_DIR = 1;
    //EMERGENCY_BUTTON_DIR = 1;

    CNPUBbits.CNPUB1 = 1;
    CNPUBbits.CNPUB11 = 1;

    RLED_DIR = 0;
    GLED_DIR = 0;
    YLED_DIR = 0;
    SYSTEM_ON_LED_DIR = 0;

    SYSTEM_ON_LED_OFF();
    LEDs_ClearAll();
}
