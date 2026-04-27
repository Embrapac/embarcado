/*
 * File:   uart.c
 * Author: Matheus Grossi
 *
 * Mapeamento de pinos (PPS):
 *   RC0  (RPC0)  ->  U1TX  (saida)
 *   RC1  (RPC1)  ->  U1RX  (entrada)
 *
 * PBCLK2 configurado explicitamente para SYSCLK/1 = 8 MHz.
 * BRG (BRGH=1) = (8 000 000 / (4 * 115 200)) - 1 = 16  (erro 2,1 %)
 */

#include <xc.h>
#include <stdint.h>
#include "defs.h"
#include "uart.h"

#define PBCLK2_HZ   SYSCLK_HZ
#define UART1_BRG   ((uint16_t)((PBCLK2_HZ / (4UL * baudrate)) - 1UL))

#define PPS_U1TX_RC0    0x01u
#define PPS_U1RX_RC1    0x07u

/* Macro de unlock/lock do SYSKEY */
#define SYSKEY_UNLOCK() do { SYSKEY = 0x00000000u; \
                             SYSKEY = 0xAA996655u; \
                             SYSKEY = 0x556699AAu; } while(0)
#define SYSKEY_LOCK()   do { SYSKEY = 0x33333333u; } while(0)

void uart1_init(void)
{
    /* ----------------------------------------------------------
     * 1. PB2DIV e protegido — precisa de SYSKEY unlock.
     *    Sem isso a escrita e ignorada e PBCLK2 fica em SYSCLK/2,
     *    causando baud rate errado (BRG calculado para 8 MHz mas
     *    clock real seria 4 MHz → baud real = 58.824 bps).
     * ---------------------------------------------------------- */
    SYSKEY_UNLOCK();
    PB2DIVbits.PBDIV = 0;           /* PBCLK2 = SYSCLK / 1 = 8 MHz */
    SYSKEY_LOCK();
    while (!PB2DIVbits.PBDIVRDY);   /* aguarda estabilizar           */

    /* ----------------------------------------------------------
     * 2. Desbloquear PPS (IOLOCK — tambem precisa de SYSKEY)
     * ---------------------------------------------------------- */
    SYSKEY_UNLOCK();
    CFGCONbits.IOLOCK = 0;
    SYSKEY_LOCK();

    /* ----------------------------------------------------------
     * 3. Mapear pinos
     * ---------------------------------------------------------- */
    RPC0R = PPS_U1TX_RC0;           /* RC0 = U1TX (saida)            */
    U1RXR = PPS_U1RX_RC1;           /* U1RX lido de RC1 (entrada)    */

    /* ----------------------------------------------------------
     * 4. Rebloquear PPS
     * ---------------------------------------------------------- */
    SYSKEY_UNLOCK();
    CFGCONbits.IOLOCK = 1;
    SYSKEY_LOCK();

    /* ----------------------------------------------------------
     * 5. Desabilitar funcao analogica em RC0 (AN6) e RC1 (AN7)
     * ---------------------------------------------------------- */
    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;

    /* ----------------------------------------------------------
     * 6. Direcao dos pinos (UART sobrescreve apos ON=1,
     *    mas configurar antes e boa pratica)
     * ---------------------------------------------------------- */
    TRISCbits.TRISC0 = 0;           /* RC0: saida (TX)               */
    TRISCbits.TRISC1 = 1;           /* RC1: entrada (RX)             */

    /* ----------------------------------------------------------
     * 7. Configurar UART1
     * ---------------------------------------------------------- */
    U1MODE = 0;
    U1BRG  = UART1_BRG;             /* BRG = 16 com PBCLK2 = 8 MHz  */

    U1MODEbits.BRGH  = 1;           /* 4x clock                      */
    U1MODEbits.PDSEL = 0;           /* 8 bits, sem paridade          */
    U1MODEbits.STSEL = 0;           /* 1 stop bit                    */

    U1STA = 0;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;

    U1MODEbits.ON = 1;
}

void uart1_send_byte(uint8_t data)
{
    while (U1STAbits.UTXBF);
    U1TXREG = data;
    while (!U1STAbits.TRMT);
}
