#include "defs.h"
#include "adc_1.h"

void __ISR(_TIMER_2_VECTOR, IPL1AUTO) Timer2Handler(void)
{
    ADCCON3bits.GSWTRG = 1;
    IFS0bits.T2IF = 0;
}

void __ISR(_ADC_EOS_VECTOR, IPL2AUTO) EOS_Handler(void)
{
    volatile unsigned int temp = ADCCON2;

    g_adc0_last_value = (uint16_t)ADCDATA0;
    g_adc0_sample_count++;
    g_adc0_new_sample = 1u;

    (void)temp;
    IFS3bits.AD1EOSIF = 0;
}
