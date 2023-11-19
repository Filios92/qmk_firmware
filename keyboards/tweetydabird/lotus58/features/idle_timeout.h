#include "action.h"

typedef enum {
    IDLE_TIMEOUT_TIMER_INC = 0,
    IDLE_TIMEOUT_TIMER_DEC,
    IDLE_TIMEOUT_TIMER_TOG,
} idle_timeout_keycodes_base_t;

void idle_timeout_sleep_cb(void);
void idle_timeout_wakeup_cb(void);

void idle_timeout_process_idle(void);
void idle_timeout_process_wakeup(keyrecord_t *record);

void idle_timeout_increase_timer(void);
void idle_timeout_decrease_timer(void);
void idle_timeout_toggle(void);

void idle_timeout_process_keycode(idle_timeout_keycodes_base_t keycode, keyrecord_t *record);
