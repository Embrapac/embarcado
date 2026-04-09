/* 
 * File:   main.c
 * Author: matheus
 *
 * Created on 8 de Abril de 2026, 20:05
 */

#include <stdio.h>
#include <stdlib.h>
#include <defs.h>


//--------------------------------------------------------------
// Definições importantes:
#define PORTLED PORTB
#define FOSC 12000000UL // Frequência do oscilador (12 MHz)
#define BAUDRATE 9600   // Taxa de baud desejada
#define DIVIDER ((int)(FOSC/(16UL * BAUDRATE))-1)   // Cálculo do divisor para a taxa de baud
#define CTS _RF12                                   // Clear To Send, input
#define RTS _RF13                                   // Request To Send, output
#define TRTS TRISFbits.TRISF13                      // Tris control for RTS pin
//--------------------------------------------------------------
// Variáveis globais:
uint8_t counter_timer1 = 0;
uint8_t timer1_final = 25;
uint8_t accept = 0
//--------------------------------------------------------------
// Declara funções do módulo:
void init_timer1(void);
void call_init_timer1(void);


void init_usart(void) {
    // UART Configuration for 9600 bps at 4 MHz:
    SPBRG = DIVIDER;    // Baud rate setting for 9600 bps at 4 MHz
    TXSTA = 0b00100100; // Transmit Enable, Asynchronous mode
    RXSTA = 0b10010000; // Serial Port Enable, Continuous Receive Enable

    PIE1 |= 0b00100000; // Enable USART Receive Interrupt
    PIR1 &= 0b11011111; // Clear USART Receive Interrupt Flag
}

void init_timer0(void) {

    /***************************************************************************/
    //Timer 0 habilita em:
    //4Mhz/4 = 1MHz -> 1us por incremento
    //1us * 64 = 64us por incremento (prescaler 1):
    /***************************************************************************/
    T0CS = 0
    PSA = 0;            // Assign prescaler to Timer0
    PS2 = 1;            // Prescaler 1:64
    PS1 = 0;
    PS0 = 0;
    TMR0 = 0;           // Clear Timer0 register
    INTCON |= 0x20;
}

void call_init_timer1(void) {
    
    counter_timer0++;                       // Each 10s increment the counter 
    if(counter_timer0 >= timer0_final) {    // If sum == 25ms
        counter_timer0 = 0;                 // Reset counter
        accept = 1;                         // Set accept flag to 1
    }

    TRM0 = 0x64;                            // Timer0 -> reload       

}

void main(void) {

    PORTA = 0;                              // Clear PORTA
    PORTB = 0;                              // Clear PORTB

    INTCON = 0x00;                          // Disable all interrupts
    CMCON = 0x07;                           // Disable comparators

    TRISA = 0xFF;                           // PORTA as input
    TRISB = 0x00;                           // PORTB as output

    PORTLED &= ~0x10;                       // Clear bit 4 of PORTB (turn off LED)

    init_timer0();                          // Initialize Timer0    
    init_usart();                           // Initialize USART

    /***************************************************************************************/
        INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
        INTCONbits.GIE = 1;                         // Enable global interrupts
    /**************************************************************************************/

    while (1)
    {
        if(accept) {                            // If accept flag is set
            // What do you wanna do:
            PORTLED ^= 0x10;                    // Toggle bit 4 of PORTB (toggle LED)
            accept = 0;                         // Reset accept flag
        }
    }
    
    return;
}

