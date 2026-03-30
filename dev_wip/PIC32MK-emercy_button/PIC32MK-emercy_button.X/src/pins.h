#ifndef PINS_H
#define PINS_H

#include "defs.h"

#define BT_LIG PORTBbits.RB1
#define BT_EME PORTBbits.RB2

#define led_verd_state LATBbits.LATB10
#define led_verm_state LATBbits.LATB13
#define led_amar_state LATBbits.LATB4

/**
 * @brief Inicializa GPIOs e o estado inicial do sistema.
 */

void init(void);

#endif // PINS_H
