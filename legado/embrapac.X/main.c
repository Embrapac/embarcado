#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// ===================================================================
// CONFIG BITS
// ===================================================================
#pragma config FWDTEN = OFF        // Disable Watchdog
#pragma config POSCMOD = HS        // External crystal
#pragma config FNOSC = SPLL        // Use PLL
#pragma config FPLLICLK = PLL_FRC
#pragma config FPLLIDIV = DIV_1
#pragma config FPLLMULT = MUL_50
#pragma config FPLLODIV = DIV_2

// Delay simples
static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms * 48000 / 2; i++)
        __asm__ volatile("nop");
}

int main(void)
{
    //Definição do tipo do pinout 0-Saida // 1-Entrada
    TRISAbits.TRISA10 = 0;

    while(1)
    {
        // LIGA O LED
        LATBbits.LATA10 ^= 1; 
        delay_ms(1000);
    
                // LIGA O LED
        LATBbits.LATA10 ^= 1; 
        delay_ms(1000);
    }

    return 0;
}