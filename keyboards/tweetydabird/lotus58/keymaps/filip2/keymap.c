#include QMK_KEYBOARD_H
#include "keymap_polish.h"
#include "transactions.h"

#include "layers.h"
#include "features/idle_timeout.h"
#include "features/key_press_delay.h"

// extern rgblight_config_t rgblight_config;
extern uint8_t highlight_layer_keys;

bool     lock_layer_state = false;

typedef union {
  uint32_t raw;
  struct {
    uint8_t highlight_layer_keys;
  };
} user_config_t;

user_config_t user_config;

typedef struct _master_to_slave_t {
    uint32_t m2s_data;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    uint32_t s2m_data;
} slave_to_master_t;

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    // slave_to_master_t *s2m = (slave_to_master_t*)out_data;
    // s2m->s2m_data = m2s->m2s_data + 5; // whatever comes in, add 5 so it can be sent back
    highlight_layer_keys = (uint8_t)m2s->m2s_data;
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
    user_config.raw = eeconfig_read_user();
    if (user_config.highlight_layer_keys) {
        highlight_layer_keys = user_config.highlight_layer_keys;
    }
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
    IT_TINC,
    IT_TDEC,
    IT_TTOG,
    RGB_TOG_NOEEP,
    RGB_HL_KEYS,
    KPD_TOG, // toggle key press delay
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

#define KCH_Z          KC_Z

// 
// Combos
// 
#define FOR_EACH_COMBO(_GENERATE) \
_GENERATE(FO_CMB_UNDO,      LCTL(KC_Z),     KCH_Z, KC_X, COMBO_END) \
_GENERATE(FO_CMB_ALTTAB,    KC_NO,          KCH_F, KC_G, COMBO_END) \
_GENERATE(FO_CMB_CTLTAB,    KC_NO,          KC_V, KC_B, COMBO_END) \
_GENERATE(FO_CMB_COPY_CUT,  KC_COPY_CUT,    KC_X, KC_C, COMBO_END) \
_GENERATE(FO_CMB_PASTE,     LCTL(KC_V),     KC_C, KC_V, COMBO_END) \
_GENERATE(FO_CMB_ARROW_L,   KC_ARROW_L,     S(KC_LBRC), S(KC_RBRC), COMBO_END) \
_GENERATE(FO_CMB_ARROW_R,   KC_ARROW_R,     S(KC_RBRC), KC_FO_TILD, COMBO_END) \
_GENERATE(FO_CMB_ESC,       KC_ESC,         KC_H, KCH_J, COMBO_END) \
_GENERATE(FO_CMB_ESC2,      KC_ESC,         KCH_A, KC_Q, COMBO_END) \
_GENERATE(FO_CMB_TAB,       KC_TAB,         KC_G, KC_T, COMBO_END) \
_GENERATE(FO_CMB_SHIFT_TAB, LSFT(KC_TAB),   KCH_F, KC_R, COMBO_END) \
_GENERATE(FO_CMB_BKSPC,     KC_BACKSPACE,   KC_H, KC_Y, COMBO_END) \
_GENERATE(FO_CMB_DEL,       KC_DEL,         KCH_J, KC_U, COMBO_END) \
_GENERATE(FO_CMB_MOUSE,     TT(_MOUSE),     KC_W, KC_R, COMBO_END) \

// === //
#define _GENERATE_ENUM(index, _, ...) index,
#define _GENERATE_COMBO(index, _, ...) const uint16_t PROGMEM combo_ ## index [] = {__VA_ARGS__};
#define _GENERATE_KEY_COMBOS(index, keycode, ...) [index] = COMBO(combo_ ## index, keycode),
enum combos { FOR_EACH_COMBO(_GENERATE_ENUM) COMBO_LENGTH };
FOR_EACH_COMBO(_GENERATE_COMBO);
combo_t key_combos[COMBO_LENGTH] = { FOR_EACH_COMBO(_GENERATE_KEY_COMBOS) };
// === //

uint8_t combo_ref_from_layer(uint8_t layer){
    switch (get_highest_layer(layer_state)){
        case _MYRALT: return _MYRALT;
        default: return _QWERTY;
    }
    return layer;  // important if default is not in case.
}

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
    if (index == FO_CMB_MOUSE)
        return true;
    return false;
}

uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!


void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        // Delays added, some issue since Win11...
        case FO_CMB_ALTTAB:
            if (pressed) {
                register_mods(MOD_LALT);
                wait_ms(10);
                tap_code_delay(KC_TAB, 20);
            } else {
                wait_ms(10);
                unregister_mods(MOD_LALT);
            }
            break;
        case FO_CMB_CTLTAB:
            if (pressed) {
                register_mods(MOD_LCTL);
                wait_ms(10);
                tap_code_delay(KC_TAB, 20);
            } else {
                wait_ms(10);
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
                        KC_LGUI, TT(_NUM), LSFT_T(KC_BSPC), KC_SPC,     FO_ENT,  MO(_MYRALT),  LT(_FN, KC_TAB), KC_ESC
),

[_GAME] = LAYOUT(
 KC_ESC,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_MUTE,     KC_NO,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_DEL,
 KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T,                      KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
 KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G,                      KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
 KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_MSC,     KC_RSET, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
            KC_LGUI, TT(_NUM), KC_LALT, KC_SPC,     FO_ENT, FO_RALT, LT(_FN, KC_TAB), KC_ESC
),

[_GAME2] = LAYOUT(
 KC_ESC,  KC_1, KC_2, KC_3, KC_4, KC_5, KC_MUTE,     KC_NO,  KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_DEL,
 KC_T, KC_TAB,  KC_Q, KC_W, KC_E, KC_R,                       KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_BSLS,
 KC_G, KC_LCTL, KC_A, KC_S, KC_D, KC_F,                       KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
 KC_B, KC_LSFT, KC_Z, KC_X, KC_C, KC_V,  KC_MSC,     KC_RSET, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
            KC_LGUI, TT(_NUM), KC_LALT, KC_SPC,     FO_ENT, FO_RALT, LT(_FN, KC_TAB), KC_ESC
),

[_NUM] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
 _______, _______, _______, _______, _______, _______,                       KC_EQL,  KC_7,    KC_8,    KC_9,    KC_0,    _______,
 _______, _______, _______, _______, _______, TG(_FN),                       KC_MINS, KC_4,    KC_5,    KC_6,    KC_ASTR, _______,
 _______, _______, _______, _______, _______, _______, _______,     _______, KC_PLUS, KC_1,    KC_2,    KC_3,    KC_SLSH, KC_RSET,
                            _______, _______, _______, _______,     _______, LT(_MYRALT, KC_0), KC_COMM, KC_DOT
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

[_NAV] = LAYOUT(
 KC_RSET, _______, _______, _______, _______, _______, _______,     _______, _______,  _______,  _______, _______,    _______, TG(_SYS),
 _______, _______, _______, _______, _______, _______,                       KC_PGUP,  KC_HOME,    KC_UP,  KC_END,    _______, _______,
 _______, _______, _______, _______, _______, _______,                       KC_PGDN,  KC_LEFT,  KC_DOWN, KC_RGHT, TG(_MOUSE), KC_PSCR,
 _______, _______, _______, _______, _______, _______, _______,     KC_LCKL, _______, FO_PVTAB, FO_NXTAB, _______,    _______, KC_INS,
                            _______, _______, _______, _______,     KC_ENT,  _______,  _______,  _______
),

[_MYRALT] = LAYOUT(
 _______, _______, _______, _______, _______,   _______, _______,    _______, _______,   _______,   _______,   _______, _______, _______,
 _______, S(KC_1), S(KC_2), PL_EOGO, S(KC_4),S(KC_BSLS),                       KC_EQL,   KC_LBRC,   KC_RBRC,   PL_OACU, _______, _______,
 _______, PL_AOGO, PL_SACU, S(KC_3), S(KC_8),   S(KC_5),                      KC_MINS,   S(KC_9),   S(KC_0),   PL_LSTR, _______, _______,
 _______, PL_ZDOT, PL_ZACU, PL_CACU, S(KC_7),   S(KC_6), _______,    _______, PL_NACU,   KC_LCBR,   KC_RCBR,KC_FO_TILD, KC_BSLS, KC_RALT,
                           _______, _______, _______, S(KC_MINS),    _______, _______, _______, _______
),

[_MOUSE] = LAYOUT(
 KC_RSET, _______, _______, MS_ACL2, _______, _______, _______,      _______, _______, _______, _______, _______, _______, _______,
 _______, _______, _______, MS_ACL1, _______, _______,                        MS_WHLU, MS_BTN1, MS_UP,   MS_BTN2, _______, _______,
 _______, _______, _______, MS_ACL0, _______, _______,                        MS_WHLD, MS_LEFT, MS_DOWN, MS_RGHT, MS_ACL1, MS_ACL2,
 _______, _______, _______, _______, _______, _______, _______,      _______, MS_BTN4, MS_BTN5, MS_BTN3, _______, MS_ACL0, _______,
                            _______, _______, _______, _______,      MS_BTN1, MS_BTN2, _______, _______
),

[_SYS] = LAYOUT(
 KC_RSET, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, _______,     _______, QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, UG_HUEU, UG_TOGG,
 QK_BOOT, XXXXXXX, KC_QWERTY, XXXXXXX, XXXXXXX, AS_TOGG,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, UG_SATU, UG_NEXT,
 XXXXXXX, XXXXXXX, KPD_TOG,   DB_TOGG, XXXXXXX, KC_CAPS,                   RGB_HL_KEYS, XXXXXXX, XXXXXXX, XXXXXXX, UG_VALU, UG_SPDU,
 XXXXXXX, KC_PWR,  XXXXXXX,   XXXXXXX, XXXXXXX, CM_TOGG, _______, TO(_GAME), TO(_GAME2), XXXXXXX, XXXXXXX, KC_RSFT, IT_TINC, IT_TTOG,
                              _______, _______, _______, _______,     _______, _______, _______, _______ 
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
            tap_code(clockwise ? KC_VOLD : KC_VOLU);
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
        case TT(_NUM):
            return 50;
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
        case KC_LPRN:
        case KC_RPRN:
        case KC_LBRC:
        case KC_RBRC:
        case KC_LCBR:
        case KC_RCBR:
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    idle_timeout_process_wakeup(record);

    const uint8_t mod_state = get_mods();

#if defined(KEY_PRESS_DELAY_OLED_ENABLE)
    key_press_delay_update(record);
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
        case IT_TINC:
        case IT_TDEC:
        case IT_TTOG:
            idle_timeout_process_keycode(keycode - IT_TINC, record);
            break;
        case RGB_HL_KEYS:
            if (record->event.pressed)
            {
                highlight_layer_keys = (highlight_layer_keys + 1) % _MAX_LAYERS;
                user_config.highlight_layer_keys = highlight_layer_keys;
                eeconfig_update_user(user_config.raw);
            }
            break;
        // case RGB_TOG_NOEEP:
        //     if (record->event.pressed)
        //     {
        //         // fo_idle_timer = 0;
        //         rgblight_config.enable ? rgblight_suspend() : rgblight_wakeup();
        //     }
        //     break;
        case KPD_TOG:
            if (record->event.pressed)
                key_press_delay_enabled = !key_press_delay_enabled;
            break;
    }

    // Else process normally
    return true;
}

void housekeeping_task_user(void) {
    idle_timeout_process_idle();
    if (is_keyboard_master()) {
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 500) {
            master_to_slave_t m2s = {highlight_layer_keys};
            if (transaction_rpc_send(USER_SYNC_A, sizeof(m2s), &m2s)) {
                last_sync = timer_read32();
            }
        }
    }
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

#if defined(CHORDAL_HOLD)
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record)
{
    // Thumb row
    if (tap_hold_record->event.key.row == 9 || tap_hold_record->event.key.row == 4) {
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
    if (other_keycode == TT(_NUM) || other_keycode == LT(_MYRALT, KC_0)) {
        return true;
    }
    if (tap_hold_keycode == FO_RALT) {
        return true;
    }
    
    // if (tap_hold_keycode == KCH_J || tap_hold_keycode == KCH_K) {
    //     return false;
    // }

    return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case KCH_J:
        // case KCH_K:
        //     return true;
        default:
            return false;
    }
}



