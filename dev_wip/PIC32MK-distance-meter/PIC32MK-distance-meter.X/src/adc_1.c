#include "adc_1.h"
#include "out_control.h"

volatile uint16_t g_adc0_last_value = 0u;
volatile uint32_t g_adc0_sample_count = 0u;
volatile uint8_t g_adc0_new_sample = 0u;
volatile float g_adc_voltage = 0.0f;
volatile float g_distance_cm = 0.0f;
volatile uint8_t g_voltage_zone = 0u;

static volatile uint8_t g_measurement_enabled = 0u;

void init_ADC(void)
{
    ADCCON1bits.ON = 0;

    ADC0CFG = DEVADC0;

    ADC0TIMEbits.SAMC = 100;
    ADC0TIMEbits.ADCDIV = 8;
    ADC0TIMEbits.SELRES = 0b11;

    ADCCON1bits.STRGSRC = 1;
    ADCCON3bits.CONCLKDIV = 11;
    ADCCON3bits.VREFSEL = 0;   /* AVDD / AVSS */

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

    IFS3bits.AD1D0IF = 0;
    IEC3bits.AD1D0IE = 0;

    ADCCON2bits.EOSIEN = 1;
    IPC25bits.AD1EOSIP = 2;
    IFS3bits.AD1EOSIF = 0;
    IEC3bits.AD1EOSIE = 1;

    IFS2bits.AD1IF = 0;
    IEC2bits.AD1IE = 0;

    g_measurement_enabled = 0u;
}

void analog_measurement_enable(void)
{
    g_measurement_enabled = 1u;
    g_adc0_new_sample = 0u;
    T2CONbits.ON = 1;
}

void analog_measurement_disable(void)
{
    g_measurement_enabled = 0u;
    T2CONbits.ON = 0;
    g_adc0_new_sample = 0u;
    g_voltage_zone = 0u;
    g_adc_voltage = 0.0f;
    g_distance_cm = 0.0f;
    LEDs_ClearAll();
}

uint8_t analog_measurement_is_enabled(void)
{
    return g_measurement_enabled;
}

void analog_process_sample(void)
{
    const float sensor_value = (float)g_adc0_last_value;
    const float v = (sensor_value * ADC_REFERENCE_VOLTAGE) / ADC_MAX_COUNTS;

    g_adc_voltage = v;
    g_distance_cm =
        (DIST_POLY_A * v * v * v) +
        (DIST_POLY_B * v * v) +
        (DIST_POLY_C * v) +
        DIST_POLY_D;

    /*
     * Fora da faixa válida de medição do sensor:
     * todas as saídas devem permanecer desligadas.
     */
    if ((g_distance_cm < DIST_MIN_VALID_CM) ||
        (g_distance_cm > DIST_MAX_VALID_CM))
    {
        g_voltage_zone = 0u;
        LEDs_ClearAll();
        return;
    }

    Output_UpdateFromVoltage(g_adc_voltage);
}
