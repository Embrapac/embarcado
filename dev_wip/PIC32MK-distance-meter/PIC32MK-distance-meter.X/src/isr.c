#include "defs.h"
#include "adc_1.h"
#include "pins.h"

/* Flags de evento setadas pelo ISR, lidas e zeradas pelo main() */
volatile uint8_t g_btn_on_event  = 0u;
volatile uint8_t g_btn_off_event = 0u;

/* ---- Timer2: 10 ms por tick -------------------------------------------- */
/*
 * PR2 ajustado para 10 ms (ver init_TMR2 em main.c).
 * A cada 50 ticks (500 ms) dispara a conversao ADC.
 * A cada tick faz o scanning de botoes com debounce por integrador:
 *   - Pressionado : incrementa ate BTN_DEBOUNCE_COUNT
 *   - Solto       : decrementa ate 0
 *   - Borda de subida confirmada : seta flag de evento (uma vez por press)
 */
void __ISR(_TIMER_2_VECTOR, IPL1AUTO) Timer2Handler(void)
{
    static uint8_t s_adc_tick = 0u;

    /* --- Disparo do ADC a cada 50 ticks (500 ms) ------------------------- */
    s_adc_tick++;
    if (s_adc_tick >= 50u)
    {
        s_adc_tick = 0u;
        ADCCON3bits.GSWTRG = 1;
    }

    /* --- Debounce botao ON — RB11 ---------------------------------------- */
    {
        static uint8_t s_on_count     = 0u;
        static uint8_t s_on_confirmed = 0u;
        uint8_t on_now;

        if (PWR_BTN == BTN_PRESSED_LEVEL)
        {
            if (s_on_count < BTN_DEBOUNCE_COUNT) { s_on_count++; }
        }
        else
        {
            if (s_on_count > 0u) { s_on_count--; }
        }

        on_now = (s_on_count >= BTN_DEBOUNCE_COUNT) ? 1u : 0u;
        if ((on_now != 0u) && (s_on_confirmed == 0u))
        {
            g_btn_on_event = 1u;
        }
        s_on_confirmed = on_now;
    }

    /* --- Debounce botao OFF — RB4 ---------------------------------------- */
    {
        static uint8_t s_off_count     = 0u;
        static uint8_t s_off_confirmed = 0u;
        uint8_t off_now;

        if (OFF_BTN == BTN_PRESSED_LEVEL)
        {
            if (s_off_count < BTN_DEBOUNCE_COUNT) { s_off_count++; }
        }
        else
        {
            if (s_off_count > 0u) { s_off_count--; }
        }

        off_now = (s_off_count >= BTN_DEBOUNCE_COUNT) ? 1u : 0u;
        if ((off_now != 0u) && (s_off_confirmed == 0u))
        {
            g_btn_off_event = 1u;
        }
        s_off_confirmed = off_now;
    }

    IFS0bits.T2IF = 0;
}

/* ---- ADC EOS: le resultado da conversao --------------------------------- */
void __ISR(_ADC_EOS_VECTOR, IPL2AUTO) EOS_Handler(void)
{
    volatile unsigned int temp = ADCCON2;

    g_adc0_last_value   = (uint16_t)ADCDATA0;
    g_adc0_sample_count++;
    g_adc0_new_sample   = 1u;

    (void)temp;
    IFS3bits.AD1EOSIF = 0;
}
