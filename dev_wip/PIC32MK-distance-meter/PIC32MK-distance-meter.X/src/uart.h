#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART_REMOTE_TOGGLE_COMMAND    0x20u

#define UART_RX_EVENT_NONE            0x00u
#define UART_RX_EVENT_ACTIVITY        0x01u
#define UART_RX_EVENT_REMOTE_TOGGLE   0x02u

void    uart1_init(void);
void    uart1_send_byte(uint8_t data);
void    uart1_send_string(const char *text);
uint8_t uart1_rx_poll(void);
void    uart1_led_service_1ms(void);

#endif /* UART_H */
