#ifndef PINS_H
#define PINS_H

#include "defs.h"

/* ---- LEDs de sinalização de distância ----------------------------------- */
#define RLED            LATBbits.LATB13
#define RLED_DIR        TRISBbits.TRISB13

#define GLED            LATBbits.LATB15
#define GLED_DIR        TRISBbits.TRISB15

#define YLED            LATAbits.LATA1
#define YLED_DIR        TRISAbits.TRISA1

/*
 * Botão de liga/desliga — RB11
 *
 * POLARIDADE: defina conforme a sua ligação de hardware.
 *
 *   Ativo-baixo (padrão): botão conecta RB11 ao GND → pull-up interno ativo.
 *     BTN_PRESSED_LEVEL  0u
 *     PWR_BTN_CNPU       habilitado em Pins_Init()
 *
 *   Ativo-alto: botão conecta RB11 a VDD → sem pull-up interno.
 *     Altere BTN_PRESSED_LEVEL para 1u e comente a linha do CNPU.
 */
#define BTN_PRESSED_LEVEL   1u          /* ativo-alto: RB11 vai a VCC ao pressionar */

#define PWR_BTN             PORTBbits.RB11
#define PWR_BTN_DIR         TRISBbits.TRISB11
#define PWR_BTN_CNPU        CNPUBbits.CNPUB11

/*
 * Botão de desligar — RB4 (ativo-alto, mesmo padrão de hardware do PWR_BTN)
 * Pressionar desliga o sistema incondicionalmente.
 */
#define OFF_BTN             PORTBbits.RB4
#define OFF_BTN_DIR         TRISBbits.TRISB4

/*
 * LED de status do sistema — RB10
 * Aceso enquanto o sistema de medição estiver ativo.
 */
#define PWR_LED             LATBbits.LATB10
#define PWR_LED_DIR         TRISBbits.TRISB10

void Pins_Init(void);

#endif /* PINS_H */
