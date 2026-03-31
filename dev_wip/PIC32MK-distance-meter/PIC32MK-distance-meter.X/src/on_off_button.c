#include "on_off_button.h"

#include "emergency_button.h"
#include "pins.h"

static system_state_t state = STATE_OFF;
static int bt_lig_prev = 0;
static int bt_eme_prev = 0;

void on_off_button_init(void)
{
    bt_lig_prev = 0;
    bt_eme_prev = 0;
    state = STATE_OFF;

    mode_off();
}

void mode_off(void)
{
    LED_ON = 0;
    LED_OFF = 1;
    LED_EMERGENCY = 0;
}

void mode_on(void)
{
    LED_ON = 1;
    LED_OFF = 0;
    LED_EMERGENCY = 0;
}

void operation(void)
{
    int bt_lig_now = BT_LIG;
    int bt_eme_now = BT_EME;

    if (emergency_button_check_activation(state, bt_eme_now, &bt_eme_prev) != 0U) {
        state = STATE_EMERGENCY;
    }

    if (state == STATE_EMERGENCY) {
        (void)emergency_button_handle(&state, bt_lig_now, &bt_lig_prev);
        return;
    }

    /* Liga/desliga normal por borda de subida */
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