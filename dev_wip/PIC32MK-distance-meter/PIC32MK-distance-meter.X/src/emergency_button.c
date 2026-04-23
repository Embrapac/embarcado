#include "emergency_button.h"

#include "on_off_button.h"
#include "pins.h"

uint8_t emergency_button_check_activation(system_state_t current_state,
                                          int bt_eme_now,
                                          int *bt_eme_prev)
{
    if ((current_state == STATE_ON) && (bt_eme_now == 1) && (*bt_eme_prev == 0))
    {
        delay_ms(20);

        if (BT_EME == 1)
        {
            *bt_eme_prev = bt_eme_now;
            return 1U;
        }
    }

    *bt_eme_prev = bt_eme_now;
    return 0U;
}

uint8_t emergency_button_handle(system_state_t *current_state,
                                int bt_lig_now,
                                int *bt_lig_prev)
{
    if ((bt_lig_now == 1) && (*bt_lig_prev == 0))
    {
        delay_ms(20);

        if (BT_LIG == 1)
        {
            *current_state = STATE_ON;
            mode_on();

            *bt_lig_prev = BT_LIG;
            return 1U;
        }
    }

    *bt_lig_prev = bt_lig_now;
    mode_sos_step();

    return 0U;
}

void mode_sos_step(void)
{
    static uint8_t emergency_blink = 0u;

    LED_ON = 0;
    LED_OFF = 1;

    emergency_blink = !emergency_blink;
    LED_EMERGENCY = emergency_blink;

    delay_ms(150);
}
