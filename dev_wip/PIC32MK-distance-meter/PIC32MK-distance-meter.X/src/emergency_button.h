#ifndef EMERGENCY_BUTTON_H
#define EMERGENCY_BUTTON_H

#include "defs.h"

/**
 * @brief Verifica a solicitação de entrada no modo de emergência.
 *
 * @param current_state Estado atual do sistema.
 * @param bt_eme_now Leitura atual do botão de emergência.
 * @param bt_eme_prev Ponteiro para o estado anterior do botão de emergência.
 *
 * @return 1 quando a transição para emergência for validada; caso contrário, 0.
 */

uint8_t emergency_button_check_activation(system_state_t current_state,
                                          int bt_eme_now,
                                          int *bt_eme_prev);

/**
 * @brief Processa o comportamento do sistema enquanto está em emergência.
 *
 * @param current_state Ponteiro para o estado atual do sistema.
 * @param bt_lig_now Leitura atual do botão liga/desliga.
 * @param bt_lig_prev Ponteiro para o estado anterior do botão liga/desliga.
 *
 * @return 1 quando sai do modo de emergência; caso contrário, 0.
 */

uint8_t emergency_button_handle(system_state_t *current_state,
                                int bt_lig_now,
                                int *bt_lig_prev);

/**
 * @brief Executa um passo do modo de emergência.
 */

void mode_sos_step(void);

#endif // EMERGENCY_BUTTON_H
