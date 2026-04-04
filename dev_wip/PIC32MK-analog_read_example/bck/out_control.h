#ifndef OUT_CONTROL_H
#define OUT_CONTROL_H

/* Faixa válida de medição do sensor em centímetros. */
#define DIST_MIN_VALID_CM       20.0f
#define DIST_MAX_VALID_CM       80.0f

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

extern volatile uint8_t g_voltage_zone;

void LEDs_ClearAll(void);
void update_led_output(float voltage, float distance_cm);

#endif /* OUT_CONTROL_H */
