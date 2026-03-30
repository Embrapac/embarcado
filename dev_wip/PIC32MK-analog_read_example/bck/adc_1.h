#ifndef ADC_1_H
#define ADC_1_H

#include <stdint.h>

#define ADC_USED_CHANNEL        0u
#define ADC_USED_LABEL          "AN0"
#define ADC_REFERENCE_VOLTAGE   3.3f
#define ADC_MAX_COUNTS          4095.0f

/* Curva polinomial da distância em função da tensão do sensor:
 * f(V) = 1.720473*V^3 + 4.15228*V^2 - 59.8489*V + 117.313
 */

#define DIST_POLY_A             1.720473f
#define DIST_POLY_B             4.15228f
#define DIST_POLY_C            -59.8489f
#define DIST_POLY_D             117.313f

extern volatile uint16_t g_adc0_last_value;
extern volatile uint32_t g_adc0_sample_count;
extern volatile uint8_t  g_adc0_new_sample;
extern volatile float    g_adc_voltage;
extern volatile float    g_distance_cm;

void init_TMR2(void);
void init_ADC(void);
void analog_process_sample(void);

#endif /* ADC_1_H */
