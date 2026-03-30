#define _CONFIG_BITS_SOURCE
#include "defs.h"
#include "adc_1.h"
#include "pins.h"
#include "out_control.h"

void init_OSC(void)
{
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    /* PBCLK2 = SYSCLK / 1 */
    PB2DIVbits.PBDIV = 0;

    SYSKEY = 0x33333333;
}

void init_TMR2(void)
{
    /*
     * Timer2 — tick de 10 ms para scanning de botoes.
     * PBCLK2 = 8 MHz, prescaler 1:64 (TCKPS = 6), modo 16 bits (T32 = 0)
     * PR2 = (10ms * 8 000 000 / 64) - 1 = 1249
     *
     * O disparo do ADC e feito dentro do ISR a cada 50 ticks (500 ms).
     */
    T2CONbits.ON    = 0;
    T2CONbits.TCKPS = 6;   /* 1:64 */
    T2CONbits.TCS   = 0;
    T2CONbits.T32   = 0;   /* modo 16 bits — nao usa Timer3 */
    PR2             = 1249u;

    IPC2bits.T2IP  = 1;
    IFS0bits.T2IF  = 0;
    IEC0bits.T2IE  = 1;

    T2CONbits.ON = 1;
}

int main(void)
{
    uint8_t system_on = 0u;   /* 0 = desligado  |  1 = ligado */

    Pins_Init();
    init_OSC();
    init_TMR2();
    init_ADC();

    INTCONSET = _INTCON_MVEC_MASK;
    __builtin_enable_interrupts();

    while (1)
    {
        /* ---- Botao LIGAR - RB11 ----------------------------------------- */
        if (g_btn_on_event != 0u)
        {
            g_btn_on_event = 0u;

            if (system_on == 0u)
            {
                system_on = 1u;
                PWR_LED   = 1u;
            }
        }

        /* ---- Botao DESLIGAR - RB4 --------------------------------------- */
        if (g_btn_off_event != 0u)
        {
            g_btn_off_event = 0u;

            if (system_on != 0u)
            {
                system_on = 0u;
                PWR_LED   = 0u;
                LEDs_ClearAll();
            }
        }

        /* ---- Processa amostra ADC (apenas com sistema ligado) ----------- */
        if ((system_on != 0u) && (g_adc0_new_sample != 0u))
        {
            g_adc0_new_sample = 0u;
            analog_process_sample();
        }
    }

    return 0;
}
