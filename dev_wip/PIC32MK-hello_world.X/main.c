/* * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

// 1. DEFINA A MACRO AQUI para ativar os config bits do defs.h
#define _CONFIG_BITS_SOURCE 

#include "defs.h"

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

#define SYSCLK_HZ 12000000UL   // ajuste para o clock REAL do seu projeto

static void delay_ms(uint32_t ms)
{
    // CP0 Count geralmente incrementa a SYSCLK/2
    const uint32_t ticks_per_ms = (SYSCLK_HZ / 2U) / 1000U;
    uint32_t start = _CP0_GET_COUNT();
    uint32_t wait  = ticks_per_ms * ms;

    while ((uint32_t)(_CP0_GET_COUNT() - start) < wait) { }
}

int main(void)
{
    // garante JTAG desligado em runtime também
    CFGCONbits.JTAGEN = 0;

    // RB10 como saída usando struct
    TRISBbits.TRISB10 = 0;

    // inicia em LOW
    LATBbits.LATB10 = 0;

    while (1)
    {
        //Força nível lógico baixo:
        LATBbits.LATB10 = 0;
        
        //Tempo de espera:
        delay_ms(500);
        
        //Força nível lógico alto:
        LATBbits.LATB10 = 1;
        
        //Tempo de espera:
        delay_ms(500);
    }
}
