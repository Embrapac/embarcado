#ifndef EMERGENCY_BUTTON_H
#define EMERGENCY_BUTTON_H

#include "defs.h"

uint8_t emergency_button_check_activation(system_state_t current_state,
                                          int bt_eme_now,
                                          int *bt_eme_prev);

uint8_t emergency_button_handle(system_state_t *current_state,
                                int bt_lig_now,
                                int *bt_lig_prev);

void mode_sos_step(void);

#endif /* EMERGENCY_BUTTON_H */
