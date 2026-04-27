#define _CONFIG_BITS_SOURCE
#include "defs.h"

#include "adc_1.h"
#include "pins.h"
#include "on_off_button.h"
#include "uart.h"
#include "uart_status.h"

int main(void)
{
    uint8_t rx_events;
    uint8_t current_status;
    uint8_t last_status;

    init();
    last_status = uart_build_status_byte();

    while (1)
    {
        rx_events = uart1_rx_poll();

        if ((rx_events & UART_RX_EVENT_REMOTE_TOGGLE) != 0u)
        {
            on_off_button_request_remote_toggle();
        }

        operation();

        if ((analog_measurement_is_enabled() != 0u) &&
            (g_adc0_new_sample != 0u))
        {
            g_adc0_new_sample = 0u;
            analog_process_sample();
        }

        current_status = uart_build_status_byte();

        if (current_status != last_status)
        {
            uart1_send_byte(current_status);
            last_status = current_status;
        }

        delay_ms(1u);
    }

    return 0;
}
