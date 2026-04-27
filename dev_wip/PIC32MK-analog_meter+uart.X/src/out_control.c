#include "out_control.h"
#include "adc_1.h"
#include "pins.h"
#include "uart_status.h"

#define RLED_ON()       do { RLED = 1u; } while (0)
#define RLED_OFF()      do { RLED = 0u; } while (0)
#define GLED_ON()       do { GLED = 1u; } while (0)
#define GLED_OFF()      do { GLED = 0u; } while (0)
#define YLED_ON()       do { YLED = 1u; } while (0)
#define YLED_OFF()      do { YLED = 0u; } while (0)

void LEDs_ClearAll(void)
{
    RLED_OFF();
    GLED_OFF();
    YLED_OFF();

    g_voltage_zone = 0u;
    uart_status_set_box_code(UART_BOX_CODE_NONE);
}

void Output_UpdateFromVoltage(float voltage)
{
    if ((voltage >= GREEN_VOLTAGE_MIN) &&
        (voltage <= GREEN_VOLTAGE_MAX))
    {
        g_voltage_zone = 1u;
        RLED_OFF();
        GLED_ON();
        YLED_OFF();
        uart_status_set_box_code(UART_BOX_CODE_P);
    }
    else if ((voltage >= YELLOW_VOLTAGE_MIN) &&
             (voltage <= YELLOW_VOLTAGE_MAX))
    {
        g_voltage_zone = 2u;
        RLED_OFF();
        GLED_OFF();
        YLED_ON();
        uart_status_set_box_code(UART_BOX_CODE_M);
    }
    else if ((voltage >= RED_VOLTAGE_MIN) &&
             (voltage <= RED_VOLTAGE_MAX))
    {
        g_voltage_zone = 3u;
        RLED_ON();
        GLED_OFF();
        YLED_OFF();
        uart_status_set_box_code(UART_BOX_CODE_G);
    }
    else
    {
        LEDs_ClearAll();
    }
}
