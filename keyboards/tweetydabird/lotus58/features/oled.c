#include QMK_KEYBOARD_H
#include "layers.h"
#include "features/key_press_delay.h"

#define CRLF1 "\n"
#define CRLF2 "\n\n"
#define CRLF3 "\n\n\n"
#define CRLF4 "\n\n\n\n"
#define CRLF5 "\n\n\n\n\n"

extern uint8_t encoder_mode;

// 5 letters x 16 lines
static void print_status_narrow(void) {
#if 1
#    if defined(KEY_PRESS_DELAY_OLED_ENABLE)
    if (key_press_delay_enabled)
    {
        char     buf[8];
        uint16_t d = key_press_delay_get_val();
        snprintf(buf, 8, "%um", d);
        oled_write_ln(buf, false);
    }
#    endif

    const uint8_t l = get_highest_layer(layer_state);
    if (l < _MAX_LAYERS) {
        oled_write_P(layers_str[l], false);
    }

    oled_write_P(PSTR("\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock) {
        oled_write_ln_P(PSTR("Caps- lock"), led_usb_state.caps_lock);
    } else {
        oled_write_ln_P(PSTR(CRLF2), false);
    }

    uint8_t osl_mods = get_mods() | get_oneshot_mods();
    if (osl_mods & MOD_MASK_SHIFT) oled_write_P(PSTR("S"), true);
    if (osl_mods & MOD_MASK_CTRL) oled_write_P(PSTR("C"), true);
    if (osl_mods & MOD_MASK_ALT) oled_write_P(PSTR("A"), true);
    if (osl_mods & MOD_MASK_GUI) oled_write_P(PSTR("W"), true);
    oled_write_ln_P(PSTR(CRLF1), false);

    if (encoder_mode == ENCODER_ARROW) oled_write_ln_P(PSTR("EncAr"), false);
    if (encoder_mode == ENCODER_PAGE) oled_write_ln_P(PSTR("EncPg"), false);

#    if defined(AUTO_SHIFT_ENABLE)
    bool autoshift = get_autoshift_state();
    oled_write_P(PSTR("\n"), false);
    if (autoshift) {
        oled_write_P(PSTR("Auto-Shift"), autoshift);
    } else {
        oled_write_P(PSTR(CRLF2), false);
    }
#    endif

#    if defined(CAPS_WORD_ENABLE)
    bool caps_word = is_caps_word_on();
    oled_write_P(PSTR("\n"), false);
    if (caps_word) {
        oled_write_P(PSTR("CapsWord"), caps_word);
    } else {
        oled_write_P(PSTR(CRLF2), false);
    }
#    endif
#endif
}

static void print_config(void) {
    const uint8_t l = get_highest_layer(layer_state);
    oled_write_P(layers_str[l], false);
    // 1 line done

    extern uint16_t idle_timeout_ms;
    oled_write_ln_P(PSTR("RGB"), rgblight_is_enabled());
    if (rgblight_is_enabled() || idle_timeout_ms) {
        char buff_mode[8] = {};
        snprintf(buff_mode, 6, "M %3u", rgblight_get_mode());
        oled_write(buff_mode, false);
        snprintf(buff_mode, 6, "S %3u", rgblight_get_speed());
        oled_write(buff_mode, false);
        snprintf(buff_mode, 6, "h %3u", rgblight_get_hue());
        oled_write(buff_mode, false);
        snprintf(buff_mode, 6, "s %3u", rgblight_get_sat());
        oled_write(buff_mode, false);
        snprintf(buff_mode, 6, "v %3u", rgblight_get_val());
        oled_write(buff_mode, false);

        uint8_t rgb_timeout_s = idle_timeout_ms / 1000;
        snprintf(buff_mode, 6, "T %2us", rgb_timeout_s);
        oled_write(buff_mode, false);
        
        extern uint8_t highlight_layer_keys;
        snprintf(buff_mode, 6, "HL %2u", highlight_layer_keys);
        oled_write(buff_mode, false);
    } else {
        oled_write_ln_P(PSTR(CRLF5), false);
        oled_write_ln_P(PSTR(CRLF1), false);
    }
    // 8 lines done
}

bool oled_task_user(void) {
    if (IS_LAYER_ON(_SYS)) {
        print_config();
    } else {
        print_status_narrow();
    }
    return false;
}
