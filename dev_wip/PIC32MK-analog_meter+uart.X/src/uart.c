#include <xc.h>
#include <stdint.h>

#include "defs.h"
#include "pins.h"
#include "uart.h"

#define UART1_BRG               ((uint16_t)(((PBCLK_HZ + (2UL * UART1_BAUDRATE)) / (4UL * UART1_BAUDRATE)) - 1UL))
#define PPS_U1TX_RC0            0x01u
#define PPS_U1RX_RC1            0x06u
#define UART_LED_ON_MS          1000u

static volatile uint16_t uart1_tx_led_timer_ms = 0u;
static volatile uint16_t uart1_rx_led_timer_ms = 0u;

static void uart1_start_tx_pulse(void)
{
    UART_TX_LED = 1u;
    uart1_tx_led_timer_ms = UART_LED_ON_MS;
}

static void uart1_start_rx_pulse(void)
{
    UART_RX_LED = 1u;
    uart1_rx_led_timer_ms = UART_LED_ON_MS;
}

void uart1_init(void)
{
    SYSKEY = 0x00000000u;
    SYSKEY = 0xAA996655u;
    SYSKEY = 0x556699AAu;
    CFGCONbits.IOLOCK = 0;

    RPC0R = PPS_U1TX_RC0;
    U1RXR = PPS_U1RX_RC1;

    CFGCONbits.IOLOCK = 1;
    SYSKEY = 0x33333333u;

    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 1;

    U1MODE = 0u;
    U1STA = 0u;

    U1MODEbits.BRGH = 1u;
    U1MODEbits.PDSEL = 0u;
    U1MODEbits.STSEL = 0u;
    U1BRG = UART1_BRG;

    U1STAbits.UTXEN = 1u;
    U1STAbits.URXEN = 1u;
    U1MODEbits.ON = 1u;

    uart1_tx_led_timer_ms = 0u;
    uart1_rx_led_timer_ms = 0u;
    UART_TX_LED = 0u;
    UART_RX_LED = 0u;
}

void uart1_send_byte(uint8_t data)
{
    while (U1STAbits.UTXBF != 0u)
    {
    }

    U1TXREG = data;

    while (U1STAbits.TRMT == 0u)
    {
    }

    uart1_start_tx_pulse();
}

void uart1_send_string(const char *text)
{
    while ((text != (const char *)0) && (*text != '\0'))
    {
        uart1_send_byte((uint8_t)(*text));
        text++;
    }
}

uint8_t uart1_rx_poll(void)
{
    uint8_t rx_events = UART_RX_EVENT_NONE;

    if (U1STAbits.OERR != 0u)
    {
        U1STAbits.OERR = 0u;
    }

    while (U1STAbits.URXDA != 0u)
    {
        const uint8_t rx_data = (uint8_t)U1RXREG;

        uart1_start_rx_pulse();
        rx_events |= UART_RX_EVENT_ACTIVITY;

        if (rx_data == UART_REMOTE_TOGGLE_COMMAND)
        {
            rx_events |= UART_RX_EVENT_REMOTE_TOGGLE;
        }
    }

    return rx_events;
}

void uart1_led_service_1ms(void)
{
    if (uart1_tx_led_timer_ms > 0u)
    {
        uart1_tx_led_timer_ms--;

        if (uart1_tx_led_timer_ms == 0u)
        {
            UART_TX_LED = 0u;
        }
    }

    if (uart1_rx_led_timer_ms > 0u)
    {
        uart1_rx_led_timer_ms--;

        if (uart1_rx_led_timer_ms == 0u)
        {
            UART_RX_LED = 0u;
        }
    }
}
