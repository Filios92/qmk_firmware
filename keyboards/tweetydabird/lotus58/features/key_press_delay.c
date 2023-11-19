#include "key_press_delay.h"

static uint16_t key_press_delay_last_timer;
uint16_t key_press_delay_diff;
bool key_press_delay_enabled = false;

void key_press_delay_update(const keyrecord_t * record) {
    if (!key_press_delay_enabled)
        return;
    
    if (record->event.pressed) {
        key_press_delay_diff       = record->event.time - key_press_delay_last_timer;
        key_press_delay_last_timer = record->event.time;
    }
}

uint16_t key_press_delay_get_val(void) {
    return key_press_delay_diff > 9999 ? 9999 : key_press_delay_diff;
}
