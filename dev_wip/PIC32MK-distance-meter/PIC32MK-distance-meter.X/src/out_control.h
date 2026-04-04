#ifndef OUT_CONTROL_H
#define OUT_CONTROL_H

#include "defs.h"

/* Zonas de tensão para indicação por LED.
 * Foi adicionada uma pequena banda morta entre as faixas para reduzir
 * acionamentos espúrios por ruído próximo aos limiares.
 */
#define GREEN_VOLTAGE_MIN       0.95f
#define GREEN_VOLTAGE_MAX       1.50f
#define YELLOW_VOLTAGE_MIN      1.55f
#define YELLOW_VOLTAGE_MAX      2.10f
#define RED_VOLTAGE_MIN         2.15f
#define RED_VOLTAGE_MAX         2.50f

void LEDs_ClearAll(void);
void Output_UpdateFromVoltage(float voltage);

#endif /* OUT_CONTROL_H */
