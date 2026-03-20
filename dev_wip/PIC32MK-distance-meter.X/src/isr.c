#include "defs.h"
#include "adc_1.h"
#include "pins.h"

void __ISR(_TIMER_2_VECTOR, IPL1AUTO) Timer2Handler(void)
{
    if (g_system_state == SYSTEM_STATE_ON)
    {
        ADCCON3bits.GSWTRG = 1;
    }

    IFS0bits.T2IF = 0;
}

void __ISR(_CHANGE_NOTICE_B_VECTOR, IPL7AUTO) EmergencyButtonHandler(void)
{
    volatile uint32_t portb_snapshot = PORTB;

    if (EMERGENCY_BUTTON_PRESSED())
    {
        g_emergency_isr_count++;
        System_Stop();
    }

    (void)portb_snapshot;
    IFS1bits.CNBIF = 0;
}

void __ISR(_ADC_EOS_VECTOR, IPL2AUTO) EOS_Handler(void)
{
    volatile unsigned int temp = ADCCON2;

    if (g_system_state == SYSTEM_STATE_ON)
    {
        g_adc0_last_value = (uint16_t)ADCDATA0;
        g_adc0_sample_count++;
        g_adc0_new_sample = 1u;
    }
    else
    {
        g_adc0_new_sample = 0u;
    }

    (void)temp;
    IFS3bits.AD1EOSIF = 0;
}
