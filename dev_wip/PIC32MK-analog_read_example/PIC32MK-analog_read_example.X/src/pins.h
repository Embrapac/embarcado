#ifndef PINS_H
#define PINS_H

#include "defs.h"

#define RLED            LATBbits.LATB9
#define RLED_DIR        TRISBbits.TRISB9

#define GLED            LATBbits.LATB12
#define GLED_DIR        TRISBbits.TRISB12

#define YLED            LATBbits.LATB3
#define YLED_DIR        TRISBbits.TRISB3

void Pins_Init(void);

#endif /* PINS_H */
