#ifndef ON_OFF_BUTTON_H
#define ON_OFF_BUTTON_H

#include "defs.h"

/**
 * @brief Inicializa as variáveis de controle do botão liga/desliga.
 */

void on_off_button_init(void);

/**
 * @brief Coloca o sistema em modo desligado.
 */

void mode_off(void);

/**
 * @brief Coloca o sistema em modo ligado.
 */

void mode_on(void);

/**
 * @brief Retorna o estado atual do sistema principal.
 */

system_state_t on_off_button_get_state(void);

/**
 * @brief Trata a lógica principal dos botões e estados.
 */

void operation(void);

#endif // ON_OFF_BUTTON_H
