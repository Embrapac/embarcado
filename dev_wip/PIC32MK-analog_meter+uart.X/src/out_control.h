#ifndef OUT_CONTROL_H
#define OUT_CONTROL_H

#include "defs.h"

/* Zonas de tensão para indicação por LED.
 * Foi adicionada uma pequena banda morta entre as faixas para reduzir
 * acionamentos espúrios por ruído próximo aos limiares.
 */
#define GREEN_VOLTAGE_MIN       0.98f
#define GREEN_VOLTAGE_MAX       1.20f
#define YELLOW_VOLTAGE_MIN      1.25f
#define YELLOW_VOLTAGE_MAX      1.95f
#define RED_VOLTAGE_MIN         2.00f
#define RED_VOLTAGE_MAX         2.15f

void LEDs_ClearAll(void);
void Output_UpdateFromVoltage(float voltage);

#endif /* OUT_CONTROL_H */
