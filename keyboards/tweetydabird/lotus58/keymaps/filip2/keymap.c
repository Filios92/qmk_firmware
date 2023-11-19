#include QMK_KEYBOARD_H
#include "keymap_polish.h"
#include "features/achordion.h"

#include "layers.h"

static uint16_t fo_idle_timer    = 0;
static bool     lock_layer_state = false;

void fo_process_idle(void) {
    if (fo_idle_timer && timer_expired(timer_read(), fo_idle_timer)) {
        rgblight_suspend();
        fo_idle_timer = 0;
    }
}

void fo_process_idle_wakeup(keyrecord_t *record) {
    if (record->event.pressed) {
        if (fo_idle_timer == 0) {
            rgblight_wakeup();
        }
        fo_idle_timer = (record->event.time + 20000) | 1;
    }
}

void keyboard_post_init_user(void) {
#if defined(AUTO_SHIFT_ENABLE)
    autoshift_disable();
#endif
}

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_NUMERIC,
    KC_FUNCTION,
    KC_SYSTEM,
    KC_ARROW_L,
    KC_ARROW_R,
    KC_FO_PGUP_END,
    KC_FO_PGDN_HOME,
    KC_FO_COPY_CAT,
    KC_FO_COPY_CUT,
    KC_FO_TILD,
    KC_ENCM, // Change encoder mode
    KC_LCKL // Lock layer (Nav)
};

#define FO_RALT         LT(_MYRALT, KC_DELETE)
#define FO_ENT          LT(_NAV, KC_ENT)
#define KC_MSC          LT(0, KC_MEDIA_PLAY_PAUSE)
#define KC_PGUP_END     LT(0, KC_FO_PGUP_END)
#define KC_PGDN_HOME    LT(0, KC_FO_PGDN_HOME)
#define KC_COPY_CAT     LT(0, KC_FO_COPY_CAT)
#define KC_COPY_CUT     LT(0, KC_FO_COPY_CUT)
#define KC_RSET         TO(_QWERTY)
#define FO_NXTAB        C(KC_PGDN)
#define FO_PVTAB        C(KC_PGUP)
// Home row
#define KCH_A   LGUI_T(KC_A)
#define KCH_S   LALT_T(KC_S)
#define KCH_D   LCTL_T(KC_D)
#define KCH_F   LSFT_T(KC_F)
#define KCH_J   RSFT_T(KC_J)
#define KCH_K   RCTL_T(KC_K)
#define KCH_L   LALT_T(KC_L)
#define KCH_SC  LGUI_T(KC_SCLN)

#define KCH_Z          LSFT_T(KC_Z)

// 
// Combos
// 
enum combos {
  FO_CMB_UNDO,
  FO_CMB_ALTTAB,
  FO_CMB_CTLTAB,
  FO_CMB_COPY_CUT,
  FO_CMB_PASTE,
  FO_CMB_ARROW_L,
  FO_CMB_ARROW_R,
  FO_CMB_ESC,
  FO_CMB_ESC2,
  FO_CMB_TAB,
  FO_CMB_SHIFT_TAB,
  FO_CMB_BKSPC,
  FO_CMB_DEL,
  COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM combo_undo[]        = {KCH_Z, KC_X, COMBO_END};
const uint16_t PROGMEM combo_alttab[]      = {KCH_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_ctltab[]      = {KC_V, KC_B, COMBO_END};
const uint16_t PROGMEM combo_copy_cut[]    = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_paste[]       = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo_arrow_left[]  = {S(KC_LBRC), S(KC_RBRC), COMBO_END};
const uint16_t PROGMEM combo_arrow_right[] = {KC_FO_TILD,    KC_BSLS, COMBO_END};
const uint16_t PROGMEM combo_esc[]         = {KC_H, KCH_J, COMBO_END};
const uint16_t PROGMEM combo_esc2[]        = {KCH_A, KC_Q, COMBO_END};
const uint16_t PROGMEM combo_tab[]         = {KC_G, KC_T, COMBO_END};
const uint16_t PROGMEM combo_shift_tab[]   = {KCH_F, KC_R, COMBO_END};
const uint16_t PROGMEM combo_bkspc[]       = {KC_H, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_del[]         = {KCH_SC, KC_P, COMBO_END};

combo_t key_combos[COMBO_LENGTH] = {
    [FO_CMB_UNDO]      = COMBO(combo_undo, LCTL(KC_Z)),
    [FO_CMB_ALTTAB]    = COMBO(combo_alttab, KC_NO /* LALT(KC_TAB) */),
    [FO_CMB_CTLTAB]    = COMBO(combo_ctltab, KC_NO /* LALT(KC_TAB) */),
    [FO_CMB_COPY_CUT]  = COMBO(combo_copy_cut, KC_COPY_CUT),
    [FO_CMB_PASTE]     = COMBO(combo_paste, LCTL(KC_V)),
    [FO_CMB_ARROW_L]   = COMBO(combo_arrow_left, KC_ARROW_L),
    [FO_CMB_ARROW_R]   = COMBO(combo_arrow_right, KC_ARROW_R),
    [FO_CMB_ESC]       = COMBO(combo_esc, KC_ESC),
    [FO_CMB_ESC2]      = COMBO(combo_esc2, KC_ESC),
    [FO_CMB_TAB]       = COMBO(combo_tab, KC_TAB),
    [FO_CMB_SHIFT_TAB] = COMBO(combo_shift_tab, LSFT(KC_TAB)),
    [FO_CMB_BKSPC]     = COMBO(combo_bkspc, KC_BACKSPACE),
    [FO_CMB_DEL]       = COMBO(combo_del, KC_DEL),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case FO_CMB_ALTTAB:
            if (pressed) {
                register_mods(MOD_LALT);
                tap_code(KC_TAB);
            } else {
                unregister_mods(MOD_LALT);
            }
            break;
        case FO_CMB_CTLTAB:
            if (pressed) {
                register_mods(MOD_LCTL);
                tap_code(KC_TAB);
            } else {
                unregister_mods(MOD_LCTL);
            }
            break;
    }
}

bool process_combo_key_repress(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) {
    switch (combo_index) {
        case FO_CMB_ALTTAB:
            switch (keycode) {
                case KCH_F:
                    tap_code16(S(KC_TAB));
                    return true;
                case KC_G:
                    tap_code(KC_TAB);
                    return true;
            }
        case FO_CMB_CTLTAB:
            switch (keycode) {
                case KC_V:
                    tap_code16(S(KC_TAB));
                    return true;
                case KC_B:
                    tap_code(KC_TAB);
                    return true;
            }
    }
    return false;
}

// 
// Keymap
// 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
 KC_ESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_ENCM,    XXXXXXX, KC_6,   KC_7,  KC_8,    KC_9,   KC_0,    KC_DEL,
 KC_TAB,        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,   KC_U,  KC_I,    KC_O,   KC_P,    KC_BSLS,
 CTL_T(KC_GRV), KCH_A,   KCH_S,   KCH_D,   KCH_F,   KC_G,                        KC_H,   KCH_J, KCH_K,   KCH_L,  KCH_SC,  RCTL_T(KC_QUOT),
 OSM(MOD_LSFT), KCH_Z,   KC_X,    KC_C,    KC_V,    KC_B,   KC_MSC,     KC_F24,  KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, OSM(MOD_RSFT),
                        KC_LGUI, TT(_NUM), LSFT_T(KC_BSPC), KC_SPC,     FO_ENT,  FO_RALT,  LT(_FN, KC_TAB), KC_ESC
),

[_NUM] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
 _______, _______, _______, _______, _______, _______,                       KC_EQL,  KC_7,    KC_8,    KC_9,    KC_0,    _______,
 _______, _______, _______, _______, _______, TG(_FN),                       KC_MINS, KC_4,    KC_5,    KC_6,    KC_ASTR, _______,
 _______, _______, _______, _______, _______, _______, _______,     _______, KC_PLUS, KC_1,    KC_2,    KC_3,    KC_SLSH, KC_RSET,
                            _______, _______, _______, _______,     _______, KC_0,    KC_COMM, KC_DOT
),

[_NAV] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, _______,     _______, _______,  _______,  _______, _______,    _______, TG(_SYS),
 _______, _______, _______, _______, _______, _______,                       KC_PGUP,  KC_HOME,    KC_UP,  KC_END,    _______, _______,
 _______, _______, _______, _______, _______, _______,                       KC_PGDN,  KC_LEFT,  KC_DOWN, KC_RGHT, TG(_MOUSE), KC_PSCR,
 _______, _______, _______, _______, _______, _______, _______,     KC_LCKL, _______, FO_PVTAB, FO_NXTAB, _______,    _______, KC_INS,
                            _______, _______, _______, _______,     _______, _______,  _______,  _______
),

[_FN] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, TG(_SYS),    _______, _______, _______, _______, _______, _______, _______,
 _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______,                       _______, _______, _______, _______, _______, _______,
 _______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   TG(_FN2),                     TG(_NUM), _______, _______, _______, _______, _______,
 _______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______,     _______, _______, _______, _______, _______, _______, _______,
                            _______, KC_LALT, _______, KC_RSET,     _______, _______, _______, _______
),

[_FN2] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, TG(_SYS),    _______, _______, _______, _______, _______, _______, _______,
 _______, _______, KC_F13,  KC_F14,  KC_F15,  _______,                       _______, _______, _______, _______, _______, _______,
 _______, _______, KC_F16,  KC_F17,  KC_F18,  _______,                       _______, _______, _______, _______, _______, _______,
 _______, _______, KC_F19,  KC_F20,  KC_F21,  _______, _______,     _______, _______, _______, _______, _______, _______, _______,
                            _______, _______, _______, _______,     _______, _______, _______, _______
),

[_MYRALT] = LAYOUT(
 _______, _______, _______, _______, _______,   _______, _______,    _______, _______,   _______,   _______,   _______, _______, _______,
 _______, S(KC_1), S(KC_2), PL_EOGO, S(KC_4),S(KC_BSLS),                       KC_EQL,   KC_LBRC,   KC_RBRC,   PL_OACU, _______, _______,
 _______, PL_AOGO, PL_SACU, S(KC_3), S(KC_8),   S(KC_5),                      KC_MINS,   S(KC_9),   S(KC_0),   PL_LSTR, _______, _______,
 _______, PL_ZDOT, PL_ZACU, PL_CACU, S(KC_7),   S(KC_6), _______,    _______, PL_NACU,   KC_LCBR,   KC_RCBR,KC_FO_TILD, KC_BSLS, KC_RALT,
                           _______, _______, _______, S(KC_MINS),    _______, _______, _______, _______
),

[_GAME] = LAYOUT(
 KC_ESC,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_MUTE,     KC_NO,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_DEL,
 KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T,                      KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
 KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G,                      KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
 KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_MSC,     KC_RSET, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
            KC_LGUI, TT(_NUM), KC_LALT, KC_SPC,     FO_ENT, FO_RALT, LT(_FN, KC_TAB), KC_ESC
),

// [_GAME] = LAYOUT(
//  KC_ESC,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_MUTE,     KC_NO,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_DEL,
//  KC_T, KC_TAB,  KC_Q, KC_W, KC_E, KC_R,                      KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
//  KC_G, KC_LCTL, KC_A, KC_S, KC_D, KC_F,                      KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
//  KC_B, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_MSC,     KC_RSET, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
//    LT(_FN, KC_LGUI), TT(_NUM), KC_LALT, KC_SPC,     FO_ENT, FO_RALT, LT(_FN, KC_TAB), KC_ESC
// ),

[_SYS] = LAYOUT(
 KC_RSET, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, _______,     _______, QK_BOOT, XXXXXXX, XXXXXXX, RGB_MOD, RGB_SAI, RGB_TOG,
 QK_BOOT, XXXXXXX, KC_QWERTY, XXXXXXX, XXXXXXX, AS_TOGG,                       KC_SLEP, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_SAD, RGB_M_P,
 XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, KC_CAPS,                       XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_VAI, RGB_M_B,
 XXXXXXX, KC_PWR,  XXXXXXX,   XXXXXXX, XXXXXXX, CM_TOGG, _______,   TO(_GAME), XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUD, RGB_VAD, RGB_M_R,
                              _______, _______, _______, _______,     _______, _______, _______, _______ 
),

[_MOUSE] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
 _______, _______, _______, _______, _______, _______,                        KC_WH_U, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______,
 _______, _______, _______, _______, _______, _______,                        KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, KC_ACL1, KC_ACL2,
 _______, _______, _______, _______, _______, _______, _______,      _______, KC_BTN4, KC_BTN5, KC_BTN3, _______, KC_ACL0, _______,
                            _______, _______, _______, _______,      _______, KC_RSET, _______, _______
),

};

uint8_t encoder_mode = 0;

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

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (!(keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX)) return false;

    // Process only for all mod-tap keycodes.
    switch (keycode) {
        case KCH_A:
        case KCH_S:
        case KCH_D:
        case KCH_F:
        case KCH_J:
        case KCH_K:
        case KCH_L:
        case KCH_SC:
        case KCH_Z:
            // Do not force the mod-tap key press to be handled as a modifier
            // if any other key was pressed while the mod-tap key is held down.
            return false;
        default:
            // Force the mod-tap key press to be handled as a modifier if any
            // other key was pressed while the mod-tap key is held down.
            return true;
    }
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case SFT_T(KC_SPC):
        // return TAPPING_TERM + 1250;
        case FO_ENT:
            return 180;
        case LT(_FN, KC_TAB):
        case LSFT_T(KC_BACKSPACE):
            return 150;
        case FO_RALT:
            return 50;
        default:
            return TAPPING_TERM;
    }
}

// Returns true -> process rest
bool process_num_word(uint16_t keycode, keyrecord_t *record) {
    static uint8_t is_held = 0;

    if (keycode == TT(_NUM)) {
        if (record->tap.count == 0) {
            is_held = record->event.pressed;
        }
    }

#define THIS_NUM_LAYER _NUM
    if (IS_LAYER_OFF(THIS_NUM_LAYER) || is_held) {
        return true;
    }

    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            // case QK_ONE_SHOT_MOD ... QK_TAP_DANCE_MAX:
            if (record->tap.count == 0) return true;
            keycode = keycode & 0xFF;
    }

    switch (keycode) {
        case KC_1 ... KC_0:
        case KC_PERC:
        case KC_BACKSPACE:
        case KC_PLUS:
        case KC_MINUS:
        case KC_EQUAL:
        case KC_SLASH:
        case KC_ASTERISK:
        case KC_UNDERSCORE:
        case KC_DOT:
        case KC_COMMA:
        case KC_LEFT_ALT:
        case KC_RIGHT_ALT:
        case KC_LEFT_SHIFT:
        case KC_RIGHT_SHIFT:
        case KC_LEFT_GUI:
        case KC_RIGHT_GUI:
        case KC_LEFT_CTRL:
        case KC_RIGHT_CTRL:
            break; // dont disable

        default: // disable
            if (record->event.pressed) {
                // layer_off(THIS_NUM_LAYER);
                layer_clear();
            }
    }

    return true;
}

static bool fo_tap_hold_simple_tap(uint16_t kc_tap, uint16_t kc_hold, keyrecord_t *record) {
    if (record->tap.count && record->event.pressed) { // tap
        tap_code16(kc_tap);
        return false;
    } else if (record->event.pressed) { // hold
        tap_code16(kc_hold);
        return false;
    }
    return true;
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

#if 0
static bool process_tap_or_long_press_key_register(keyrecord_t *record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) { // Key is being held.
        if (record->event.pressed) {
            register_code16(long_press_keycode);
        } else {
            unregister_code16(long_press_keycode);
        }
        return false; // Skip default handling.
    }
    return true; // Continue default handling.
}
#endif

#if defined(FO_SHOW_KEY_PRESS_DELAY)
static uint16_t my_last_timer;
static uint16_t my_diff;
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    fo_process_idle_wakeup(record);

    if (!process_achordion(keycode, record)) {
        return false;
    }

    uint8_t mod_state = get_mods();

#if defined(FO_SHOW_KEY_PRESS_DELAY)
    if (record->event.pressed) {
        my_diff       = record->event.time - my_last_timer;
        my_last_timer = record->event.time;
    }
#endif

    if (!process_num_word(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_ARROW_L:
            if (record->event.pressed) {
                add_mods(MOD_MASK_SHIFT);
                tap_code(KC_COMM);
                del_mods(MOD_MASK_SHIFT);
                tap_code(mod_state & MOD_MASK_SHIFT ? KC_EQL : KC_MINS);
                set_mods(mod_state);
            } else {
            }
            break;
        case KC_ARROW_R:
            if (record->event.pressed) {
                del_mods(MOD_MASK_SHIFT);
                tap_code(mod_state & MOD_MASK_SHIFT ? KC_EQL : KC_MINS);
                add_mods(MOD_MASK_SHIFT);
                tap_code(KC_DOT);
                set_mods(mod_state);
            } else {
            }
            break;
        case KC_MSC:
            return process_tap_or_long_press_key(record, mod_state & MOD_MASK_SHIFT ? KC_MEDIA_PREV_TRACK : KC_MEDIA_NEXT_TRACK);
        case KC_LCKL:
            lock_layer_state = true;
            return false;
        case FO_ENT:
            if (record->tap.count == 0 && !record->event.pressed && lock_layer_state) // on key released and not tapped
            {
                lock_layer_state = false;
                return false;
            } else
                return true;
        case KC_COPY_CAT:
            return fo_tap_hold_simple_tap(LCTL(KC_V), LCTL(KC_C), record);
        case KC_COPY_CUT:
            return fo_tap_hold_simple_tap(LCTL(KC_C), LCTL(KC_X), record);
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
        case KC_ENCM:
            if (record->event.pressed) {
                encoder_mode = (encoder_mode >= ENCODER_MODES_LEN - 1) ? 0 : encoder_mode + 1;
            }
            break;
    }

    // Else process normally
    return true;
}

void housekeeping_task_user(void) {
    fo_process_idle();
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FO_ENT:
        case KCH_A:
        case KCH_S:
        case KCH_D:
        case KCH_F:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

//
// Achordion stuff
//
void matrix_scan_user(void) {
    achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Thumb row
    if (tap_hold_record->event.key.row % (MATRIX_ROWS / 2) >= 4) {
        return true;
    }
    // Outermost column
    if (tap_hold_record->event.key.col == 0) {
        return true;
    }
    // z
    if (tap_hold_keycode == KCH_Z || tap_hold_keycode == S(KC_F9)) {
        return true;
    }
    if (other_keycode == TT(_NUM)) {
        return true;
    }
    if (tap_hold_keycode == FO_RALT) {
        return true;
    }

    return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    if (tap_hold_keycode == FO_RALT) {
        return 0;
    }
    return 1000;
}
