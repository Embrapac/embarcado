#ifndef ON_OFF_BUTTON_H
#define ON_OFF_BUTTON_H

#include "defs.h"

void on_off_button_init(void);
void mode_off(void);
void mode_on(void);
system_state_t on_off_button_get_state(void);
void on_off_button_request_remote_toggle(void);
void operation(void);

#endif /* ON_OFF_BUTTON_H */
