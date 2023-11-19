#include QMK_KEYBOARD_H
#include "layers.h"

extern uint8_t encoder_mode;

static void print_status_narrow(void) {
#if 1
    // 5 letters x 16
    // Print current layer
    #define CRLF1 "\n"
    #define CRLF2 "\n\n"
    #define CRLF3 "\n\n\n"
    #define CRLF4 "\n\n\n\n"
    #define CRLF5 "\n\n\n\n\n"
    // oled_write_P(PSTR("LAYER"), false);
    // oled_write_P(PSTR(CRLF1), false);

    #if defined(FO_SHOW_KEY_PRESS_DELAY)
    static char buf[8];
    {
        uint16_t d = my_diff > 9999 ? 9999 : my_diff;
        snprintf(buf, 4+1+1, "%um", d);
        oled_write(buf, false);
        // oled_write(get_u16_str(d, ' '), false);
        // oled_write_ln_P(PSTR("ms"), false);
    }
    #endif

    const uint8_t l = get_highest_layer(layer_state);
    if (l < _MAX_LAYERS)
    {
        oled_write_P(layers_str[l], false);
    }
  
    oled_write_P(PSTR("\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    if (led_usb_state.caps_lock)
    {
        oled_write_ln_P(PSTR("Caps- lock"), led_usb_state.caps_lock);
    }
    else
    {
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

#if defined(AUTO_SHIFT_ENABLE)
  bool autoshift = get_autoshift_state();
  oled_write_P(PSTR("\n"), false);
    if (autoshift)
    {
      oled_write_P(PSTR("Auto-Shift"), autoshift);
    }
    else
    {
      oled_write_P(PSTR(CRLF2), false);
    }
#endif

#if defined (CAPS_WORD_ENABLE)
    bool caps_word = is_caps_word_on();
    oled_write_P(PSTR("\n"), false);
    if (caps_word)
    {
      oled_write_P(PSTR("CapsWord"), caps_word);
    }
    else
    {
      oled_write_P(PSTR(CRLF2), false);
    }
#endif
#endif
}

bool oled_task_user(void) {
  print_status_narrow();
  return false;
}
