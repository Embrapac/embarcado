#include "pins.h"
#include "adc_1.h"
#include "on_off_button.h"
#include "out_control.h"
#include "uart.h"
#include "uart_status.h"

void init(void)
{
    CFGCONbits.JTAGEN = 0;

    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELD = 0x0000;

    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;

    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TRISCbits.TRISC11 = 0;
    TRISAbits.TRISA11 = 0;
    TRISAbits.TRISA12 = 0;
    LED_ON_EXTERNAL_DIR = 0;
    LED_ON_EXTERNAL = 0;

    RLED_DIR = 0;
    GLED_DIR = 0;
    YLED_DIR = 0;

    ANSELCbits.ANSC2 = 0;
    ANSELAbits.ANSA8 = 0;
    UART_TX_LED_DIR = 0;
    UART_RX_LED_DIR = 0;
    UART_TX_LED = 0;
    UART_RX_LED = 0;

    init_OSC();
    init_TMR2();
    init_ADC();
    uart1_init();
    uart_status_init();
    on_off_button_init();
    LEDs_ClearAll();

    T2CONbits.ON = 0;

    INTCONSET = _INTCON_MVEC_MASK;
    __builtin_enable_interrupts();
}
