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
#define BT_EME PORTBbits.RB2

#define led_verd_state LATBbits.LATB10
#define led_verm_state LATBbits.LATB13
#define led_amar_state LATBbits.LATB4

#define STATE_OFF        0
#define STATE_ON         1
#define STATE_EMERGENCY  2

//Modos da FSM:
int state = STATE_OFF;
int bt_lig_prev = 0;
int bt_eme_prev = 0;

/**
 * @brief Inicializa GPIOs e o estado inicial do sistema.
 */

void init(void);

/**
 * @brief Coloca o sistema em modo desligado.
 */

void mode_off(void);

/**
 * @brief Coloca o sistema em modo ligado.
 */

void mode_on(void);

/**
 * @brief Executa um passo do modo de emergência.
 */

void mode_sos_step(void);

/**
 * @brief Trata a lógica principal dos botões e estados.
 */

void operation(void);

void init(void)
{
    CFGCONbits.JTAGEN = 0;

    // Entradas
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    // Saídas
    TRISBbits.TRISB4  = 0;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB13 = 0;

    // Entradas digitais
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;

    bt_lig_prev = 0;
    bt_eme_prev = 0;
    state = STATE_OFF;

    mode_off();
}

void mode_off(void)
{
    led_verd_state = 0;
    led_verm_state = 1;
    led_amar_state = 0;
}

void mode_on(void)
{
    led_verd_state = 1;
    led_verm_state = 0;
    led_amar_state = 0;
}

void mode_sos_step(void)
{
    static uint8_t amar_state = 0;

    led_verd_state = 0;
    led_verm_state = 1;


    amar_state = !amar_state;
    led_amar_state = amar_state;

    delay_ms(150);
}

void operation(void)
{
    int bt_lig_now = BT_LIG;
    int bt_eme_now = BT_EME;

    // Só entra em emergência se o sistema já estiver ligado
    if ((state == STATE_ON) && (bt_eme_now == 1) && (bt_eme_prev == 0)) {
        delay_ms(20);

        if (BT_EME == 1) {
            state = STATE_EMERGENCY;
        }
    }

    bt_eme_prev = bt_eme_now;

    // Em emergência, só o botão de ligar reinicia o sistema
    if (state == STATE_EMERGENCY) {
        if ((bt_lig_now == 1) && (bt_lig_prev == 0)) {
            delay_ms(20);

            if (BT_LIG == 1) {
                state = STATE_ON;
                mode_on();

                bt_lig_prev = BT_LIG;
                return;
            }
        }

        bt_lig_prev = bt_lig_now;
        mode_sos_step();
        return;
    }

    // Liga/desliga normal por borda de subida
    if ((bt_lig_now == 1) && (bt_lig_prev == 0)) {
        delay_ms(20);

        if (BT_LIG == 1) {
            if (state == STATE_OFF) {
                state = STATE_ON;
                mode_on();
            }
            else if (state == STATE_ON) {
                state = STATE_OFF;
                mode_off();
            }
        }
    }

    bt_lig_prev = bt_lig_now;

    if (state == STATE_ON) {
        mode_on();
    }
    else {
        mode_off();
    }
}

int main(void)
{
    init();

    while (1) {
        operation();
    }

    return 0;
}