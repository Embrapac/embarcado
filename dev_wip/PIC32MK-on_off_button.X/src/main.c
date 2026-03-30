/* 
 * File:   main.c
 * Author: Matheus Grossi
 * Created on 13 de Fevereiro de 2026, 14:11
 */

#define _CONFIG_BITS_SOURCE

#include "defs.h"

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>

#define BT_LIG PORTBbits.RB1
#define BT_DES PORTBbits.RB2 

#define led_verd_state LATBbits.LATB10
#define led_verm_state LATBbits.LATB13
//#define led_amar_state LATBbits.LATB13

int state = 0;

int init(void){

    CFGCONbits.JTAGEN = 0;

    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB13 = 0;
    
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;

    //RB10 e RB13 nao tem modo analogico
    //ANSELBbits.ANSB10 = 0;
    //ANSELBbits.ANSB13 = 0;
    
    LATBbits.LATB10 = 0;
    LATBbits.LATB13 = 1;
}

int mode_off(void){
    delay_ms(50);
    led_verd_state = 1;
    led_verm_state = 0;
}

int mode_on(void){
    delay_ms(50);
    led_verd_state = 0;
    led_verm_state = 1;
}

int mode_sos(void){
    delay_ms(50);
    led_verd_state = 0;
    led_verm_state = 1;
}

int operation(void){
    if ( BT_DES ) { // Operacao de liga
        mode_off();
    }
    else
    if ( BT_LIG ) { // Operacao de desliga
        mode_on();
    }
}

int main(void)
{
    init();
    
    while(1)
            {
                operation();
            }
}