#include "defs.h"
#include "adc_1.h"
#include "out_control.h"

volatile uint16_t g_adc0_last_value  = 0u;
volatile uint32_t g_adc0_sample_count = 0u;
volatile uint8_t  g_adc0_new_sample  = 0u;
volatile float    g_adc_voltage      = 0.0f;
volatile float    g_distance_cm      = 0.0f;

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

    T2CONbits.ON    = 0;
    T2CONbits.TCKPS = 4;
    T2CONbits.TCS   = 0;
    T2CONbits.T32   = 1;

    IPC2bits.T2IP  = 1;
    IFS0bits.T2IF  = 0;
    IEC0bits.T2IE  = 1;

    T2CONbits.ON = 1;
}

void init_ADC(void)
{
    ADCCON1bits.ON = 0;

    ADC0CFG = DEVADC0;

    ADC0TIMEbits.SAMC   = 100;
    ADC0TIMEbits.ADCDIV = 8;
    ADC0TIMEbits.SELRES = 0b11;

    ADCCON1bits.STRGSRC  = 1;
    ADCCON3bits.CONCLKDIV = 11;
    ADCCON3bits.VREFSEL  = 0;   /* AVDD / AVSS */

    ADCCSS1 = 0x00000000;
    ADCCSS1bits.CSS0 = 1;

    ADCTRG1 = 0x00000000;
    ADCTRG1bits.TRGSRC0 = 3;

    ADCIMCON1 = 0x00000000;
    ADCIMCON2 = 0x00000000;

    ADCCON1bits.ON = 1;

    while (ADCCON2bits.BGVRRDY == 0)
    {
    }

    while (ADCCON2bits.REFFLT != 0)
    {
    }

    ADCANCONbits.ANEN0 = 1;
    while (ADCANCONbits.WKRDY0 == 0)
    {
    }

    ADCCON3bits.DIGEN0 = 1;

    IFS3bits.AD1D0IF  = 0;
    IEC3bits.AD1D0IE  = 0;

    ADCCON2bits.EOSIEN  = 1;
    IPC25bits.AD1EOSIP  = 2;
    IFS3bits.AD1EOSIF   = 0;
    IEC3bits.AD1EOSIE   = 1;

    IFS2bits.AD1IF = 0;
    IEC2bits.AD1IE = 0;
}

void analog_process_sample(void)
{
    const float sensor_value = (float)g_adc0_last_value;
    const float v = (sensor_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_COUNTS;

    g_adc_voltage  = v;
    g_distance_cm  =
        (DIST_POLY_A * v * v * v) +
        (DIST_POLY_B * v * v)     +
        (DIST_POLY_C * v)         +
        DIST_POLY_D;

    update_led_output(g_adc_voltage, g_distance_cm);
}

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
