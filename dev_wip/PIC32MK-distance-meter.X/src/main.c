#define _CONFIG_BITS_SOURCE
#include "defs.h"
#include "adc_1.h"
#include "out_control.h"
#include "pins.h"

volatile uint8_t g_system_state = SYSTEM_STATE_OFF;
volatile uint32_t g_emergency_isr_count = 0u;

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
    PR2 = 250000u - 16u;
    TMR2 = 0u;

    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 4;
    T2CONbits.TCS = 0;
    T2CONbits.T32 = 1;

    IPC2bits.T2IP = 1;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

void init_EmergencyButtonInterrupt(void)
{
    volatile uint32_t dummy_portb;

    CNCONBbits.ON = 0;
    CNCONBbits.EDGEDETECT = 1;

    CNENB = 0x0000u;
    CNNEB = 0x0000u;

    /*
     * Botão B em RB2 com pull-up interno:
     * solto = 1, pressionado = 0.
     * A interrupção deve ocorrer na borda de descida.
     */
    CNNEBbits.CNNEB2 = 1;

    IPC11bits.CNBIP = 7;
    IPC11bits.CNBIS = 0;

    CNCONBbits.ON = 1;
    dummy_portb = PORTB;
    (void)dummy_portb;
    IFS1bits.CNBIF = 0;
    IEC1bits.CNBIE = 1;
}

void System_Start(void)
{
    if (g_system_state == SYSTEM_STATE_ON)
    {
        return;
    }

    g_adc0_new_sample = 0u;
    IFS0bits.T2IF = 0;
    IFS3bits.AD1EOSIF = 0;
    TMR2 = 0u;
    g_system_state = SYSTEM_STATE_ON;
    SYSTEM_ON_LED_ON();
    T2CONbits.ON = 1;
}

void System_Stop(void)
{
    T2CONbits.ON = 0;
    g_system_state = SYSTEM_STATE_OFF;
    SYSTEM_ON_LED_OFF();

    g_adc0_last_value = 0u;
    g_adc0_new_sample = 0u;
    g_adc_voltage = 0.0f;
    g_distance_cm = 0.0f;
    g_voltage_zone = 0u;

    IFS0bits.T2IF = 0;
    IFS1bits.CNBIF = 0;
    IFS3bits.AD1EOSIF = 0;

    LEDs_ClearAll();
}

int main(void)
{
    uint8_t last_start_button_state;

    Pins_Init();
    init_OSC();
    init_TMR2();
    init_ADC();
    init_EmergencyButtonInterrupt();

    System_Stop();

    last_start_button_state =
        (START_BUTTON_PRESSED() != 0u) ? 1u : 0u;

    INTCONSET = _INTCON_MVEC_MASK;
    __builtin_enable_interrupts();

    while (1)
    {
        const uint8_t start_button_pressed =
            (START_BUTTON_PRESSED() != 0u) ? 1u : 0u;

        if ((start_button_pressed != 0u) &&
            (last_start_button_state == 0u) &&
            (g_system_state == SYSTEM_STATE_OFF))
        {
            System_Start();
        }

        last_start_button_state = start_button_pressed;

        /*
         * Mantido como redundância de segurança:
         * se por algum motivo a interrupção de emergência não chegar,
         * o laço principal também força o desligamento ao detectar RB2.
         */
        if ((g_system_state == SYSTEM_STATE_ON) &&
            (EMERGENCY_BUTTON_PRESSED() != 0u))
        {
            System_Stop();
        }

        if ((g_system_state == SYSTEM_STATE_ON) &&
            (g_adc0_new_sample != 0u))
        {
            g_adc0_new_sample = 0u;
            analog_process_sample();
        }
        else if (g_system_state == SYSTEM_STATE_OFF)
        {
            g_adc0_new_sample = 0u;
        }
    }

    return 0;
}
