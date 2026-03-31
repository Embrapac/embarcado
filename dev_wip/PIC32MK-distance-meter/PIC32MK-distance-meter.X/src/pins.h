#ifndef PINS_H
#define PINS_H

#include "defs.h"

/* -------------------------- Entradas / botoes -------------------------- */
#define BT_LIG             PORTBbits.RB1
#define BT_EME             PORTBbits.RB2

/* ----------------------- LEDs do primeiro conjunto --------------------- */
#define LED_ON             LATBbits.LATB10
#define LED_OFF            LATBbits.LATB13
#define LED_EMERGENCY      LATBbits.LATB4

/* ----------------------- LEDs do segundo conjunto ---------------------- */
#define RLED               LATBbits.LATB9
#define RLED_DIR           TRISBbits.TRISB9

#define GLED               LATBbits.LATB12
#define GLED_DIR           TRISBbits.TRISB12

#define YLED               LATBbits.LATB3
#define YLED_DIR           TRISBbits.TRISB3

/**
 * @brief Inicializa todos os GPIOs consolidados dos dois arquivos.
 *
 * - Desabilita JTAG.
 * - Configura RB1 e RB2 como entradas digitais.
 * - Configura RB4, RB10 e RB13 como saídas.
 * - Configura RB3, RB9 e RB12 como saídas.
 * - Mantém AN0 em RA0 como entrada analógica.
 * - Inicializa o módulo do botão liga/desliga.
 * - Limpa os LEDs externos.
 */

void init(void);

#endif // PINS_H