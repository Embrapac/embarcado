#ifndef ADC_1_H
#define ADC_1_H

#include "defs.h"

#define ADC_USED_CHANNEL        0u
#define ADC_USED_LABEL          "AN0"

extern volatile uint16_t g_adc0_last_value;
extern volatile uint32_t g_adc0_sample_count;
extern volatile uint8_t g_adc0_new_sample;
extern volatile float g_adc_voltage;
extern volatile float g_distance_cm;
extern volatile uint8_t g_voltage_zone;

void init_ADC(void);
void analog_process_sample(void);

#endif /* ADC_1_H */
