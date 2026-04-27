#include <xc.h>
#include "uart_con.h"

// Pragmas mínimos essenciais
#pragma config FNOSC = SPLL       // Oscilador: PLL
#pragma config FSOSCEN = OFF      // Secundário off
#pragma config POSCMOD = OFF      // Osc externo off
#pragma config FPLLIDIV = DIV_1   // PLL input divider
#pragma config FPLLMULT = MUL_24  // PLL multiplier
#pragma config FPLLODIV = DIV_2   // PLL output divider
#pragma config FPBDIV = DIV_1     // PBCLK = SYSCLK
#pragma config FWDTEN = OFF       // Watchdog off

int main(void)
{
    PPS_Config();
    UART2_Init();

    UART2_WriteString("\r\n--- PIC32MK UART2 mikroBUS ---\r\n");
    UART2_WriteString("UART inicializada em RC0/RC1.\r\n");

    while (1)
    {
        if (UART2_Available())
        {
            char c = UART2_Read();
            UART2_Write(c);      // ecoa caracter
        }
    }

    return 0;
}
