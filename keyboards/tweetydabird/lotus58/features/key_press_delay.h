#pragma once

#include "action.h"

void key_press_delay_update(const keyrecord_t * record);
uint16_t key_press_delay_get_val(void);
extern bool key_press_delay_enabled;
