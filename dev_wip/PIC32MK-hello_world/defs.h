#ifndef DEFS_H
#define DEFS_H

// Colocar config bits APENAS quando _CONFIG_BITS_SOURCE estiver definido
#ifdef _CONFIG_BITS_SOURCE

// PIC32MK0128MCA048 Configuration Bit Settings
// 'C' source line config statements

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

#endif // _CONFIG_BITS_SOURCE

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#endif // DEFS_H
