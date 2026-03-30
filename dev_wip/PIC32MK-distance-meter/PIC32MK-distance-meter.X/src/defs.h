#ifndef DEFS_H
#define DEFS_H

#ifdef _CONFIG_BITS_SOURCE

// DEVCFG3
#pragma config USERID   = 0xFFFF
#pragma config PWMLOCK  = OFF
#pragma config PGL1WAY  = ON
#pragma config PMDL1WAY = ON
#pragma config IOL1WAY  = ON

// DEVCFG2
#pragma config FPLLIDIV = DIV_8
#pragma config FPLLRNG  = RANGE_54_64_MHZ
#pragma config FPLLICLK = PLL_FRC
#pragma config FPLLMULT = MUL_128
#pragma config FPLLODIV = DIV_32
#pragma config BORSEL   = HIGH

// DEVCFG1
#pragma config FNOSC      = FRC
#pragma config DMTINTV    = WIN_127_128
#pragma config FSOSCEN    = ON
#pragma config IESO       = ON
#pragma config POSCMOD    = OFF
#pragma config OSCIOFNC   = OFF
#pragma config FCKSM      = CSECME
#pragma config WDTPS      = PS1048576
#pragma config WDTSPGM    = STOP
#pragma config WINDIS     = NORMAL
#pragma config FWDTEN     = OFF
#pragma config FWDTWINSZ  = WINSZ_25
#pragma config DMTCNT     = DMT31
#pragma config FDMTEN     = OFF

// DEVCFG0
#pragma config DEBUG      = OFF
#pragma config JTAGEN     = OFF
#pragma config ICESEL     = ICS_PGx1
#pragma config TRCEN      = OFF
#pragma config BOOTISA    = MIPS32
#pragma config FECCCON    = ECC_DECC_DISABLE_ECCON_WRITABLE
#pragma config FSLEEP     = OFF
#pragma config DBGPER     = PG_ALL
#pragma config SMCLR      = MCLR_NORM
#pragma config SOSCGAIN   = G3
#pragma config SOSCBOOST  = ON
#pragma config POSCGAIN   = G3
#pragma config POSCBOOST  = ON
#pragma config POSCFGAIN  = G3
#pragma config POSCAGCDLY = AGCRNG_x_25ms
#pragma config POSCAGCRNG = ONE_X
#pragma config POSCAGC    = Automatic
#pragma config EJTAGBEN   = NORMAL

// DEVCP
#pragma config CP = OFF

// SEQ
#pragma config TSEQ = 0xFFFF
#pragma config CSEQ = 0xFFFF

#endif /* _CONFIG_BITS_SOURCE */

#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>

#define ADC_REFERENCE_VOLTAGE   3.3f
#define ADC_MAX_COUNTS          4095.0f

#define DIST_POLY_A             1.720473f
#define DIST_POLY_B             4.15228f
#define DIST_POLY_C            -59.8489f
#define DIST_POLY_D             117.313f

#define DIST_MIN_VALID_CM       10.0f
#define DIST_MAX_VALID_CM       80.0f

/*
 * Debounce por integrador no ISR do Timer2 (tick = 10 ms).
 * 5 ticks = 50 ms de sinal estavel para confirmar um pressionamento.
 */
#define BTN_DEBOUNCE_COUNT      5u

/* Flags de evento setadas pelo ISR, lidas e zeradas pelo main(). */
extern volatile uint8_t g_btn_on_event;
extern volatile uint8_t g_btn_off_event;

void init_OSC(void);
void init_TMR2(void);

#endif /* DEFS_H */
