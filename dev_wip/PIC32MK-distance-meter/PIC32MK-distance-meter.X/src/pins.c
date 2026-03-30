#include "pins.h"
#include "out_control.h"

void Pins_Init(void)
{
    /*
     * AN0 no MCA048 está em RA0 — mantido analógico.
     * Todos os demais pinos do banco iniciam como digital.
     */
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;

    /* AN0 / RA0 — entrada analógica para o sensor de distância */
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;

    /* LEDs de sinalização de distância — saídas digitais */
    RLED_DIR = 0;
    GLED_DIR = 0;
    YLED_DIR = 0;
    LEDs_ClearAll();

    /*
     * RB11 — botão de ligar (ativo-alto, sem pull-up interno).
     * O botão conecta o pino ao GND em repouso e a VCC ao pressionar.
     */
    PWR_BTN_DIR = 1;
    /* PWR_BTN_CNPU = 1 — NÃO habilitar: botão tem GND fixo em repouso */

    /* RB4 — botão de desligar (mesmo padrão de hardware, ativo-alto) */
    OFF_BTN_DIR = 1;

    /* RB10 — LED de status do sistema, saída digital, apagado inicialmente */
    PWR_LED_DIR = 0;
    PWR_LED     = 0;
}
