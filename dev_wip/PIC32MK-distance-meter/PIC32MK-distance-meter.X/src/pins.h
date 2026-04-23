#ifndef PINS_H
#define PINS_H

#include "defs.h"

/* -------------------------- Entradas / botoes -------------------------- */
#define BT_LIG             PORTBbits.RB1
#define BT_EME             PORTBbits.RB2

/* ----------------------- LEDs do primeiro conjunto --------------------- */
#define LED_ON             LATCbits.LATC11
#define LED_OFF            LATAbits.LATA11
#define LED_EMERGENCY      LATAbits.LATA12

/* ----------------------- LEDs do segundo conjunto ---------------------- */
#define RLED               LATBbits.LATB9
#define RLED_DIR           TRISBbits.TRISB9

#define GLED               LATAbits.LATA1
#define GLED_DIR           TRISAbits.TRISA1

#define YLED               LATBbits.LATB3
#define YLED_DIR           TRISBbits.TRISB3

/* -------------------------- LEDs reais da UART ------------------------- */
#define UART_TX_LED        LATCbits.LATC2
#define UART_TX_LED_DIR    TRISCbits.TRISC2

#define UART_RX_LED        LATAbits.LATA8
#define UART_RX_LED_DIR    TRISAbits.TRISA8

void init(void);

#endif /* PINS_H */
