#ifndef UART_STATUS_H
#define UART_STATUS_H

#include <stdint.h>

#define UART_BOX_CODE_NONE      0u
#define UART_BOX_CODE_P         1u
#define UART_BOX_CODE_M         2u
#define UART_BOX_CODE_G         3u

void    uart_status_init(void);
void    uart_status_set_box_code(uint8_t box_code);
uint8_t uart_status_get_box_code(void);
void    uart_status_notify_state_changed(void);
uint8_t uart_build_status_byte(void);

#endif /* UART_STATUS_H */
