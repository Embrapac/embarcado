#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <xc.h>
#include <sys/attribs.h>

/*
 * Parametros operacionais compartilhados do projeto consolidado.
 *
 * Base de clock herdada do projeto UART:
 *   - clock externo da placa = 12 MHz
 *   - PBCLK assumido = SYSCLK / 2
 */
#define UART1_BAUDRATE          9600UL
#define SYSCLK_HZ               12000000UL
#define PBCLK_HZ                (SYSCLK_HZ / 2UL)
#define CORE_TIMER_HZ           (SYSCLK_HZ / 2UL)

/*
 * Timer 2 em 32 bits.
 * Mantido para gerar a periodicidade de amostragem do sistema de medicao.
 */
#define TMR2_PRESCALE_VALUE     256UL
#define TMR2_PRESCALE_BITS      7u
#define MEASUREMENT_PERIOD_MS   1000UL

/* Parametros do ADC e da curva do sensor. */
#define ADC_REFERENCE_VOLTAGE   3.3f
#define ADC_MAX_COUNTS          4095.0f

#define DIST_POLY_A             1.720473f
#define DIST_POLY_B             4.15228f
#define DIST_POLY_C            -59.8489f
#define DIST_POLY_D             117.313f

/* Faixa valida do sistema de medicao original. */
#define DIST_MIN_VALID_CM       10.0f
#define DIST_MAX_VALID_CM       80.0f

/* Faixas especificas para o empacotamento UART. */
#define BOX_MIN_CM              2.0f
#define BOX_P_MAX_CM            6.0f
#define BOX_M_MAX_CM            12.0f
#define BOX_MAX_CM              80.0f

/*
 * Mapeamento direto do classificador original por zonas de tensao.
 *
 * Sem reinterpretar distancia ou altura da caixa:
 *   - g_voltage_zone = 0 -> nenhuma caixa
 *   - g_voltage_zone = 1 -> primeira faixa  -> caixa P
 *   - g_voltage_zone = 2 -> segunda faixa   -> caixa M
 *   - g_voltage_zone = 3 -> terceira faixa  -> caixa G
 *
 * Isso segue estritamente a ordem produzida pelo classificador original.
 */
#define BOX_ZONE_NONE           0u
#define BOX_ZONE_P              1u
#define BOX_ZONE_M              2u
#define BOX_ZONE_G              3u

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
#pragma config FNOSC      = POSC
#pragma config DMTINTV    = WIN_127_128
#pragma config FSOSCEN    = ON
#pragma config IESO       = ON
#pragma config POSCMOD    = EC
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

typedef enum
{
    STATE_OFF = 0,
    STATE_ON,
    STATE_EMERGENCY
} system_state_t;

void init_OSC(void);
void init_TMR2(void);
void delay_ms(uint32_t ms);

#endif /* DEFS_H */
