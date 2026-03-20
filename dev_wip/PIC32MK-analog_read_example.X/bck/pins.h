#ifndef PINS_H
#define PINS_H

#define RLED            LATBbits.LATB13
#define RLED_DIR        TRISBbits.TRISB13

#define GLED            LATBbits.LATB15
#define GLED_DIR        TRISBbits.TRISB15

#define YLED            LATAbits.LATA1
#define YLED_DIR        TRISAbits.TRISA1

#define RLED_ON()       do { RLED = 1u; } while (0)
#define RLED_OFF()      do { RLED = 0u; } while (0)
#define GLED_ON()       do { GLED = 1u; } while (0)
#define GLED_OFF()      do { GLED = 0u; } while (0)
#define YLED_ON()       do { YLED = 1u; } while (0)
#define YLED_OFF()      do { YLED = 0u; } while (0)

void init_pins(void);

#endif /* PINS_H */
