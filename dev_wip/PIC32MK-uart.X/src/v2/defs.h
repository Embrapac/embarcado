#ifndef DEFS_H
#define DEFS_H

/*
 * Parametros operacionais — visiveis para todos os arquivos .c
 * que incluem este header (independente de _CONFIG_BITS_SOURCE).
 */
#define operation   1
#define baudrate    115200UL
#define SYSCLK_HZ   8000000UL   /* FRC interno do PIC32MK = 8 MHz */

/* ----------------------------------------------------------------
 * Config bits — gerados APENAS quando main.c define a macro
 * _CONFIG_BITS_SOURCE antes de incluir este header.
 * ---------------------------------------------------------------- */
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
#pragma config TRCEN      = ON
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

/* Declaracoes de funcoes compartilhadas */
void pins_init(void);
void blink_led(void);
void delay_ms(uint32_t ms);

#endif /* DEFS_H */