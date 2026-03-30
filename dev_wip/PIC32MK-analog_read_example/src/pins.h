#ifndef PINS_H
#define PINS_H

#include "defs.h"

#define RLED            LATBbits.LATB13
#define RLED_DIR        TRISBbits.TRISB13

#define GLED            LATBbits.LATB15
#define GLED_DIR        TRISBbits.TRISB15

#define YLED            LATAbits.LATA1
#define YLED_DIR        TRISAbits.TRISA1

void Pins_Init(void);

#endif /* PINS_H */
