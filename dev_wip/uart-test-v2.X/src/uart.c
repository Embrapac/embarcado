/*
 * File:   uart.c
 * Author: Matheus Grossi
 * Brief:  Inicializa UART1 e fornece rotinas de envio.
 *
 * Mapeamento de pinos (PPS):
 *   RC0  (RPC0)  ->  U1TX  (saida)
 *   RC1  (RPC1)  ->  U1RX  (entrada)
 *
 * Observacoes:
 *   - Nesta aplicacao, o clock do sistema e externo de 12 MHz.
 *   - O barramento periferico e assumido como PBCLK = SYSCLK.
 *   - A UART opera em 9600 8N1.
 *   - A linha TX retorna naturalmente ao estado idle (nivel alto)
 *     ao final da transmissao de cada frame UART.
 */

#include <xc.h>
#include <stdint.h>
#include "defs.h"
#include "uart.h"

// Assume PBCLK igual ao clock do sistema.
#define PBCLK_HZ            (SYSCLK_HZ / 2UL)

// BRG em modo 4x (BRGH = 1), com arredondamento para o inteiro mais proximo.
#define UART1_BRG           ((uint16_t)(((PBCLK_HZ + (2UL * baudrate)) / (4UL * baudrate)) - 1UL))

// Codigos PPS para PIC32MK0128MCA048.
#define PPS_U1TX_RC0        0x01u
#define PPS_U1RX_RC1        0x07u

/* ===============================================================
 * uart1_init
 * =============================================================== */
void uart1_init(void)
{
    // 1. Desbloquear PPS.
    SYSKEY = 0x00000000u;
    SYSKEY = 0xAA996655u;
    SYSKEY = 0x556699AAu;
    CFGCONbits.IOLOCK = 0;

    // 2. Mapear os pinos da UART1.
    RPC0R = PPS_U1TX_RC0;
    U1RXR = PPS_U1RX_RC1;

    // 3. Rebloquear PPS.
    CFGCONbits.IOLOCK = 1;
    SYSKEY = 0x33333333u;

    // 4. Garantir operacao digital nos pinos RC0 e RC1.
    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;

    // 5. Direcao basica dos pinos.
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 1;

    // 6. Configuracao da UART1.
    U1MODE = 0u;
    U1STA  = 0u;

    U1MODEbits.BRGH  = 1u;
    U1MODEbits.PDSEL = 0u;
    U1MODEbits.STSEL = 0u;
    U1BRG = UART1_BRG;

    U1STAbits.UTXEN = 1u;
    U1STAbits.URXEN = 1u;
    U1MODEbits.ON   = 1u;
}

/* ===============================================================
 * uart1_send_byte
 * Aguarda espaco no buffer TX, escreve o dado e aguarda o termino
 * da transmissao antes de retornar.
 * =============================================================== */
void uart1_send_byte(uint8_t data)
{
    while (U1STAbits.UTXBF != 0u)
    {
        // Aguarda espaco livre no buffer de transmissao.
    }

    U1TXREG = data;

    while (U1STAbits.TRMT == 0u)
    {
        // Aguarda o shift register concluir a transmissao.
    }
}

/* ===============================================================
 * uart1_send_string
 * Envia uma string ASCII terminada em '\0'.
 * Mantida para compatibilidade com testes futuros.
 * =============================================================== */
void uart1_send_string(const char *text)
{
    while ((text != (const char *)0) && (*text != '\0'))
    {
        uart1_send_byte((uint8_t)(*text));
        text++;
    }
}
