#include QMK_KEYBOARD_H
#include "layers.h"
#include "keymap_polish.h"

uint8_t encoder_mode = 0;

void keyboard_post_init_user(void) {
#if defined(AUTO_SHIFT_ENABLE)
    autoshift_disable();
#endif
}

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_FO_TILD,
};

#define KC_MSC          LT(0, KC_MEDIA_PLAY_PAUSE)

// 
// Combos
// 
#ifdef COMBO_ENABLE
enum combos {
  FO_CMB_UNDO,
  COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM combo_undo[] = {KC_Z, KC_X, COMBO_END};

combo_t key_combos[COMBO_LENGTH] = {
    [FO_CMB_UNDO]      = COMBO(combo_undo, LCTL(KC_Z)),
};
#endif

// 
// Keymap
// 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
 KC_ESC,        KC_1,   KC_2,   KC_3,   KC_4,   KC_5, KC_MPLY,         XXXXXXX, KC_6,  KC_7,  KC_8,    KC_9,   KC_0,    KC_BSPC,
 KC_TAB,        KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                           KC_Y,  KC_U,  KC_I,    KC_O,   KC_P,    KC_BSLS,
 CTL_T(KC_GRV), KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                           KC_H,  KC_J,  KC_K,    KC_L,   KC_SCLN, RCTL_T(KC_QUOT),
 KC_LSFT,       KC_Z,   KC_X,   KC_C,   KC_V,   KC_B, KC_MSC,          KC_EQL,  KC_N,  KC_M,  KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                    KC_LGUI, TT(_NUM), LALT_T(KC_BSPC), KC_SPC,     KC_ENT, TT(_MYRALT), TT(_FN_NAV), S(KC_MINUS)
),

[_FN_NAV] = LAYOUT(
 TO(_QWERTY),   KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_F5,   _______,      _______, _______, _______, _______,  KC_PSCR, KC_INS,  KC_DEL,
     _______,   KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10,                         KC_PGUP, KC_HOME, KC_UP,     KC_END, _______, _______,
     _______, _______, _______, _______, _______, KC_F11,                         KC_PGDN, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,
     _______, _______, _______, _______, _______, KC_F12,  TG(_SYS),    TG(_SYS), _______, _______, _______,  _______, _______, _______,
                                _______, _______, _______, _______,      _______, _______, _______, _______
),

[_MYRALT] = LAYOUT(
 TO(_QWERTY), _______, _______, _______, _______,   _______, _______,    _______, _______,   _______,   _______,   _______, _______, _______,
     _______, S(KC_1), S(KC_2), PL_EOGO, S(KC_4),S(KC_BSLS),                       KC_EQL,   KC_LBRC,   KC_RBRC,   PL_OACU, _______, _______,
     _______, PL_AOGO, PL_SACU, S(KC_3), S(KC_8),   S(KC_5),                      KC_MINS,   S(KC_9),   S(KC_0),   PL_LSTR, _______, _______,
     _______, PL_ZDOT, PL_ZACU, PL_CACU, S(KC_7),   S(KC_6), _______,    _______, PL_NACU,   KC_LCBR,   KC_RCBR,KC_FO_TILD, KC_BSLS, KC_RALT,
                                  _______, _______, _______, _______,    _______, _______, _______, _______
),

[_NUM] = LAYOUT(
 TO(_QWERTY), _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
     _______, _______, _______, _______, _______, _______,                       KC_EQL,  KC_7,    KC_8,    KC_9,    KC_0,    _______,
     _______, _______, _______, _______, _______, _______,                       KC_MINS, KC_4,    KC_5,    KC_6,    KC_ASTR, _______,
     _______, _______, _______, _______, _______, _______, TG(_FN_NAV), _______, KC_PLUS, KC_1,    KC_2,    KC_3,    KC_SLSH, _______,
                                _______, _______, _______, _______,     _______, KC_0,    KC_COMM, KC_DOT
),

[_SYS] = LAYOUT(
 TO(_QWERTY), XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, _______,     _______, QK_BOOT, XXXXXXX, XXXXXXX, RGB_MOD,  RGB_SAI, RGB_TOG,
     QK_BOOT, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, AS_TOGG,                       XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_SAD, RGB_M_P,
     XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, KC_CAPS,                       XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI,  RGB_VAI, RGB_M_B,
     XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, CM_TOGG, XXXXXXX, TO(_QWERTY), XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUD,  RGB_VAD, RGB_M_R,
                                 _______, _______, _______, _______,     _______, _______, _______, _______ 
),

};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN_NAV] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_MYRALT] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_SYS]    = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#else
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (encoder_mode) {
        case ENCODER_ARROW:
            tap_code(clockwise ? KC_UP : KC_DOWN);
            break;
        case ENCODER_PAGE:
            tap_code(clockwise ? KC_PGUP : KC_PGDN);
            break;
        default:
            tap_code(clockwise ? KC_VOLU : KC_VOLD);
    }
    return false;
}
#endif


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (!(keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX)) return false;

    // Process only for all mod-tap keycodes.
    switch (keycode) {
            // Do not force the mod-tap key press to be handled as a modifier
            // if any other key was pressed while the mod-tap key is held down.
            // return false;
        default:
            // Force the mod-tap key press to be handled as a modifier if any
            // other key was pressed while the mod-tap key is held down.
            return true;
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return TAPPING_TERM;
    }
}

static bool process_tap_or_long_press_key(keyrecord_t *record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) { // Key is being held.
        if (record->event.pressed) {
            tap_code16(long_press_keycode);
        }
        return false; // Skip default handling.
    }
    return true; // Continue default handling.
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t mod_state = get_mods();

    switch (keycode) {
        case KC_MSC:
            return process_tap_or_long_press_key(record, mod_state & MOD_MASK_SHIFT ? KC_MEDIA_PREV_TRACK : KC_MEDIA_NEXT_TRACK);
        case KC_FO_TILD:
            if (!record->event.pressed) {
                unregister_code16(KC_TILD);
                tap_code(KC_SPACE);
                return false;
            } else {
                register_code16(KC_TILDE);
                return false;
            }
            break;
    }

    // Else process normally
    return true;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return QUICK_TAP_TERM;
    }
}
