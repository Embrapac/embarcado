#ifndef PINS_H
#define PINS_H

#include "defs.h"

#define RLED                    LATBbits.LATB13
#define RLED_DIR                TRISBbits.TRISB13

#define GLED                    LATBbits.LATB15
#define GLED_DIR                TRISBbits.TRISB15

#define YLED                    LATAbits.LATA1
#define YLED_DIR                TRISAbits.TRISA1

#define SYSTEM_ON_LED            LATBbits.LATB10
#define SYSTEM_ON_LED_DIR        TRISBbits.TRISB10

#define START_BUTTON_PORT        PORTBbits.RB1
#define START_BUTTON_DIR         TRISBbits.TRISB1

#define EMERGENCY_BUTTON_PORT    PORTBbits.RB2
#define EMERGENCY_BUTTON_DIR     TRISBbits.TRISB2

/*
 * Botões assumidos como normalmente abertos para GND,
 * com pull-up interno habilitado.
 */
#define START_BUTTON_PRESSED()      (START_BUTTON_PORT == 0u)
#define EMERGENCY_BUTTON_PRESSED()  (EMERGENCY_BUTTON_PORT == 0u)

#define SYSTEM_ON_LED_ON()          do { SYSTEM_ON_LED = 1u; } while (0)
#define SYSTEM_ON_LED_OFF()         do { SYSTEM_ON_LED = 0u; } while (0)

void Pins_Init(void);

#endif /* PINS_H */
