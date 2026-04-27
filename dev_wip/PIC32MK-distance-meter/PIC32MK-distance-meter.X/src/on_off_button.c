#include "on_off_button.h"

#include "adc_1.h"
#include "emergency_button.h"
#include "pins.h"
#include "uart_status.h"

static system_state_t state = STATE_OFF;
static int bt_lig_prev = 0;
static int bt_eme_prev = 0;
static volatile uint8_t remote_toggle_pending = 0u;

static void on_off_button_apply_state(system_state_t new_state)
{
    state = new_state;

    if (state == STATE_ON)
    {
        mode_on();
        analog_measurement_enable();
    }
    else
    {
        mode_off();
        analog_measurement_disable();
    }

    uart_status_notify_state_changed();
}

static void on_off_button_enter_emergency_state(void)
{
    state = STATE_EMERGENCY;
    analog_measurement_disable();
    uart_status_notify_state_changed();
}

static void on_off_button_handle_toggle_request(void)
{
    if (state == STATE_OFF)
    {
        on_off_button_apply_state(STATE_ON);
    }
    else if (state == STATE_ON)
    {
        on_off_button_apply_state(STATE_OFF);
    }
    else
    {
        /* Mesmo comportamento do botao fisico durante emergencia. */
        on_off_button_apply_state(STATE_ON);
    }
}

void on_off_button_init(void)
{
    bt_lig_prev = 0;
    bt_eme_prev = 0;
    remote_toggle_pending = 0u;
    on_off_button_apply_state(STATE_OFF);
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

system_state_t on_off_button_get_state(void)
{
    return state;
}

void on_off_button_request_remote_toggle(void)
{
    remote_toggle_pending = 1u;
}

void operation(void)
{
    int bt_lig_now = BT_LIG;
    int bt_eme_now = BT_EME;
    uint8_t local_toggle_request = 0u;

    if (emergency_button_check_activation(state, bt_eme_now, &bt_eme_prev) != 0U)
    {
        on_off_button_enter_emergency_state();
    }

    if (state == STATE_EMERGENCY)
    {
        if (remote_toggle_pending != 0u)
        {
            remote_toggle_pending = 0u;
            on_off_button_handle_toggle_request();
            bt_lig_prev = bt_lig_now;
            return;
        }

        if (emergency_button_handle(&state, bt_lig_now, &bt_lig_prev) != 0U)
        {
            on_off_button_apply_state(STATE_ON);
        }

        return;
    }

    if ((bt_lig_now == 1) && (bt_lig_prev == 0))
    {
        delay_ms(20);

        if (BT_LIG == 1)
        {
            local_toggle_request = 1u;
        }
    }

    bt_lig_prev = bt_lig_now;

    if ((remote_toggle_pending != 0u) || (local_toggle_request != 0u))
    {
        remote_toggle_pending = 0u;
        on_off_button_handle_toggle_request();
        return;
    }

    if (state == STATE_ON)
    {
        mode_on();
    }
    else
    {
        mode_off();
    }
}
