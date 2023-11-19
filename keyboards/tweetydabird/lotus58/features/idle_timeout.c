#include "idle_timeout.h"

#include "timer.h"
#if defined (RGBLIGHT_ENABLE)
#include "rgblight.h"
#endif
#include "action_util.h"

#if 0
// #define IDLE_TIMEOUT_MS 20000
#else
#    define IDLE_TIMEOUT_MS idle_timeout_ms
#endif

#define IDLE_TIMEOUT_TIMER_MAX 20000
#define IDLE_TIMEOUT_TIMER_MIN 4000
#define IDLE_TIMEOUT_TIMER_STEP 2000

static uint16_t idle_timer      = 0;
uint16_t        idle_timeout_ms = 20000;

__attribute__((weak)) void idle_timeout_sleep_cb(void) {
#if defined (RGBLIGHT_ENABLE)
    rgblight_suspend();
#endif
}

__attribute__((weak)) void idle_timeout_wakeup_cb(void) {
#if defined (RGBLIGHT_ENABLE)
    rgblight_wakeup();
#endif
}

void idle_timeout_toggle(void) {
extern uint8_t rgb_matrix_timeout_enabled;
    if (idle_timeout_ms) {
        idle_timeout_ms = 0;
#if defined (RGB_MATRIX_TIMEOUT)
        rgb_matrix_timeout_enabled = false;
#endif
    }
    else {
        idle_timeout_ms = IDLE_TIMEOUT_TIMER_MAX;
#if defined (RGB_MATRIX_TIMEOUT)
        rgb_matrix_timeout_enabled = true;
#endif
    }
}

void idle_timeout_increase_timer(void) {
    if (idle_timeout_ms <= IDLE_TIMEOUT_TIMER_MAX) {
        idle_timeout_ms += IDLE_TIMEOUT_TIMER_STEP;
    }
    if (idle_timeout_ms < IDLE_TIMEOUT_TIMER_MIN || idle_timeout_ms > IDLE_TIMEOUT_TIMER_MAX) idle_timeout_ms = IDLE_TIMEOUT_TIMER_MAX;
}

void idle_timeout_decrease_timer(void) {
    if (idle_timeout_ms >= IDLE_TIMEOUT_TIMER_MIN) {
        idle_timeout_ms -= IDLE_TIMEOUT_TIMER_STEP;
    }
    if (idle_timeout_ms < IDLE_TIMEOUT_TIMER_MIN || idle_timeout_ms > IDLE_TIMEOUT_TIMER_MAX) idle_timeout_ms = IDLE_TIMEOUT_TIMER_MIN;
}

void idle_timeout_process_idle(void) {
    if (idle_timeout_ms && idle_timer && timer_expired(timer_read(), idle_timer)) {
        idle_timeout_sleep_cb();
        idle_timer = 0;
    }
}

void idle_timeout_process_wakeup(keyrecord_t *record) {
    if (record->event.pressed) {
        if (idle_timer == 0) {
            idle_timeout_wakeup_cb();
        }
        idle_timer = (record->event.time + IDLE_TIMEOUT_MS) | 1;
    }
}

void idle_timeout_process_keycode(idle_timeout_keycodes_base_t keycode, keyrecord_t *record) {
    const uint8_t shifted = get_mods() & MOD_MASK_SHIFT;

    switch (keycode) {
        case IDLE_TIMEOUT_TIMER_INC:
            if (record->event.pressed) {
                if (shifted) {
                    idle_timeout_decrease_timer();
                } else {
                    idle_timeout_increase_timer();
                }
            }
            break;
        case IDLE_TIMEOUT_TIMER_DEC:
            if (record->event.pressed) {
                if (shifted) {
                    idle_timeout_increase_timer();
                } else {
                    idle_timeout_decrease_timer();
                }
            }
            break;
        case IDLE_TIMEOUT_TIMER_TOG:
            if (record->event.pressed) idle_timeout_toggle();
            break;
        default:
            break;
    }
}
