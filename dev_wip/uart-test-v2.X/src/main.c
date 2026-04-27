/*
 * File:   main.c
 * Author: Matheus Grossi
 *
 * Comportamento:
 *   A cada 10 s, envia um unico byte 0x18 via UART1
 *   em RC0/RC1 e pisca o LED da placa.
 */

// 1. Ativa os config bits definidos em defs.h.
#define _CONFIG_BITS_SOURCE

// Libs do fabricante.
#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

// Libs do projeto.
#include "defs.h"
#include "pin_declaration.h"
#include "aux_func.h"
#include "blink.h"
#include "uart.h"

// Byte transmitido: 00011000b = 0x18.
#define UART_TX_BYTE    ((uint8_t)0x18u)

// Tempo total do ciclo em ms.
#define CICLO_MS        10000UL

// Tempo consumido por blink_led() em ms.
#define BLINK_MS        1000UL

/* =============================================================
 * main
 * ============================================================= */
int main(void)
{
    // Inicializa pinos de proposito geral.
    pins_init();

    // Inicializa UART1 (RC0 = TX, RC1 = RX, 9600 8N1).
    uart1_init();

    while (operation)
    {
        // Envia um unico byte e retorna a linha para idle ao final do frame.
        uart1_send_byte(UART_TX_BYTE);

        // Pisca o LED para sinalizar o envio.
        blink_led();

        // Aguarda o restante do ciclo de 10 s.
        delay_ms((uint32_t)(CICLO_MS - BLINK_MS));
    }

    return 0;
}
