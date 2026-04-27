/*
 * File:   uart.h
 * Author: Matheus Grossi
 * Brief:  Driver UART1 para PIC32MK0128MCA048
 *         TX -> RC0 (RPC0)  |  RX <- RC1 (RPC1)
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

void    uart1_init(void);
void    uart1_send_byte(uint8_t data);
void    uart1_send_string(const char *text);
void    uart1_rx_poll(void);
void    uart1_rx_led_service_1ms(void);

#endif /* UART_H */
