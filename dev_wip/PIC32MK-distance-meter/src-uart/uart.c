#include "uart_con.h"

void PPS_Config(void)
{
    // Destrava o sistema para configurar PPS
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    // UART2 TX -> RC0 (mikroBUS TX pin 13)
    RPC0R = 0b0010;     // U2TX function code

    // UART2 RX -> RC1 (mikroBUS RX pin 14)
    U2RXR = 0b0010;     // RC1 as U2RX

    // Trava PPS
    SYSKEY = 0x33333333;
}

void UART2_Init(void)
{
    U2MODEbits.ON = 0;      // Desliga para configurar
    U2MODEbits.BRGH = 0;    // Baud rate em modo padrão (16x)

    // BRG calculation
    U2BRG = (PBCLK2_FREQ / (16 * UART_BAUD)) - 1;

    // Habilita RX e TX
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;

    U2MODEbits.ON = 1;      // Liga UART
}

void UART2_Write(char c)
{
    while (U2STAbits.UTXBF);    // Espera espaço no buffer
    U2TXREG = c;
}

void UART2_WriteString(const char *s)
{
    while (*s)
        UART2_Write(*s++);
}

char UART2_Read(void)
{
    while (!U2STAbits.URXDA);   // Espera byte chegar
    return U2RXREG;
}

bool UART2_Available(void)
{
    return U2STAbits.URXDA;
}
