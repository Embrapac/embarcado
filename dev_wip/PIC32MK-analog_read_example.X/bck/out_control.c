#include "defs.h"
#include "pins.h"
#include "out_control.h"

volatile uint8_t g_voltage_zone = 0u;

void LEDs_ClearAll(void)
{
    RLED_OFF();
    GLED_OFF();
    YLED_OFF();
}

void update_led_output(float voltage, float distance_cm)
{
    /*
     * Fora da faixa válida de medição do sensor:
     * todas as saídas devem permanecer desligadas.
     */
    if ((distance_cm < DIST_MIN_VALID_CM) ||
        (distance_cm > DIST_MAX_VALID_CM))
    {
        g_voltage_zone = 0u;
        LEDs_ClearAll();
        return;
    }

    if ((voltage >= GREEN_VOLTAGE_MIN) &&
        (voltage <= GREEN_VOLTAGE_MAX))
    {
        g_voltage_zone = 1u;
        RLED_OFF();
        GLED_ON();
        YLED_OFF();
    }
    else if ((voltage >= YELLOW_VOLTAGE_MIN) &&
             (voltage <= YELLOW_VOLTAGE_MAX))
    {
        g_voltage_zone = 2u;
        RLED_OFF();
        GLED_OFF();
        YLED_ON();
    }
    else if ((voltage >= RED_VOLTAGE_MIN) &&
             (voltage <= RED_VOLTAGE_MAX))
    {
        g_voltage_zone = 3u;
        RLED_ON();
        GLED_OFF();
        YLED_OFF();
    }
    else
    {
        g_voltage_zone = 0u;
        LEDs_ClearAll();
    }
}
