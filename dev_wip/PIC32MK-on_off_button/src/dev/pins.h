#ifndef PINS_H
#define PINS_H

// ============================================================================
// PIC32MK0128MCA048-I/Y8X
// GPIO aliases
// dir   -> TRISx (1 = input, 0 = output)
// state -> LATx  (output latch state)
// read  -> PORTx (input pin readback)
// ============================================================================

// PORTA
#define RA0_dir         TRISAbits.TRISA0
#define RA1_dir         TRISAbits.TRISA1
#define RA4_dir         TRISAbits.TRISA4
#define RA7_dir         TRISAbits.TRISA7
#define RA8_dir         TRISAbits.TRISA8
#define RA10_dir        TRISAbits.TRISA10
#define RA11_dir        TRISAbits.TRISA11
#define RA12_dir        TRISAbits.TRISA12

#define RA0_state       LATAbits.LATA0
#define RA1_state       LATAbits.LATA1
#define RA4_state       LATAbits.LATA4
#define RA7_state       LATAbits.LATA7
#define RA8_state       LATAbits.LATA8
#define RA10_state      LATAbits.LATA10
#define RA11_state      LATAbits.LATA11
#define RA12_state      LATAbits.LATA12

#define RA0_read        PORTAbits.RA0
#define RA1_read        PORTAbits.RA1
#define RA4_read        PORTAbits.RA4
#define RA7_read        PORTAbits.RA7
#define RA8_read        PORTAbits.RA8
#define RA10_read       PORTAbits.RA10
#define RA11_read       PORTAbits.RA11
#define RA12_read       PORTAbits.RA12

// PORTB
#define RB0_dir         TRISBbits.TRISB0
#define RB1_dir         TRISBbits.TRISB1
#define RB2_dir         TRISBbits.TRISB2
#define RB3_dir         TRISBbits.TRISB3
#define RB4_dir         TRISBbits.TRISB4
#define RB5_dir         TRISBbits.TRISB5
#define RB6_dir         TRISBbits.TRISB6
#define RB7_dir         TRISBbits.TRISB7
#define RB8_dir         TRISBbits.TRISB8
#define RB9_dir         TRISBbits.TRISB9
#define RB10_dir        TRISBbits.TRISB10
#define RB11_dir        TRISBbits.TRISB11
#define RB12_dir        TRISBbits.TRISB12
#define RB13_dir        TRISBbits.TRISB13
#define RB14_dir        TRISBbits.TRISB14
#define RB15_dir        TRISBbits.TRISB15

#define RB0_state       LATBbits.LATB0
#define RB1_state       LATBbits.LATB1
#define RB2_state       LATBbits.LATB2
#define RB3_state       LATBbits.LATB3
#define RB4_state       LATBbits.LATB4
#define RB5_state       LATBbits.LATB5
#define RB6_state       LATBbits.LATB6
#define RB7_state       LATBbits.LATB7
#define RB8_state       LATBbits.LATB8
#define RB9_state       LATBbits.LATB9
#define RB10_state      LATBbits.LATB10
#define RB11_state      LATBbits.LATB11
#define RB12_state      LATBbits.LATB12
#define RB13_state      LATBbits.LATB13
#define RB14_state      LATBbits.LATB14
#define RB15_state      LATBbits.LATB15

#define RB0_read        PORTBbits.RB0
#define RB1_read        PORTBbits.RB1
#define RB2_read        PORTBbits.RB2
#define RB3_read        PORTBbits.RB3
#define RB4_read        PORTBbits.RB4
#define RB5_read        PORTBbits.RB5
#define RB6_read        PORTBbits.RB6
#define RB7_read        PORTBbits.RB7
#define RB8_read        PORTBbits.RB8
#define RB9_read        PORTBbits.RB9
#define RB10_read       PORTBbits.RB10
#define RB11_read       PORTBbits.RB11
#define RB12_read       PORTBbits.RB12
#define RB13_read       PORTBbits.RB13
#define RB14_read       PORTBbits.RB14
#define RB15_read       PORTBbits.RB15

// PORTC
#define RC0_dir         TRISCbits.TRISC0
#define RC1_dir         TRISCbits.TRISC1
#define RC2_dir         TRISCbits.TRISC2
#define RC6_dir         TRISCbits.TRISC6
#define RC7_dir         TRISCbits.TRISC7
#define RC8_dir         TRISCbits.TRISC8
#define RC9_dir         TRISCbits.TRISC9
#define RC10_dir        TRISCbits.TRISC10
#define RC11_dir        TRISCbits.TRISC11
#define RC12_dir        TRISCbits.TRISC12
#define RC13_dir        TRISCbits.TRISC13
#define RC15_dir        TRISCbits.TRISC15

#define RC0_state       LATCbits.LATC0
#define RC1_state       LATCbits.LATC1
#define RC2_state       LATCbits.LATC2
#define RC6_state       LATCbits.LATC6
#define RC7_state       LATCbits.LATC7
#define RC8_state       LATCbits.LATC8
#define RC9_state       LATCbits.LATC9
#define RC10_state      LATCbits.LATC10
#define RC11_state      LATCbits.LATC11
#define RC12_state      LATCbits.LATC12
#define RC13_state      LATCbits.LATC13
#define RC15_state      LATCbits.LATC15

#define RC0_read        PORTCbits.RC0
#define RC1_read        PORTCbits.RC1
#define RC2_read        PORTCbits.RC2
#define RC6_read        PORTCbits.RC6
#define RC7_read        PORTCbits.RC7
#define RC8_read        PORTCbits.RC8
#define RC9_read        PORTCbits.RC9
#define RC10_read       PORTCbits.RC10
#define RC11_read       PORTCbits.RC11
#define RC12_read       PORTCbits.RC12
#define RC13_read       PORTCbits.RC13
#define RC15_read       PORTCbits.RC15

// PORTD
#define RD8_dir         TRISDbits.TRISD8
#define RD8_state       LATDbits.LATD8
#define RD8_read        PORTDbits.RD8

void pin_defs(void);
void button_def(void);

#endif // PINS_H
