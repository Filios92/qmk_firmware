#include "action_layer.h"
#include "keymap_common.h"
#include "rgb_matrix.h"
#ifdef RGB_MATRIX_ENABLE

#include "info_config.h"
#include "layers.h"

#define LED_MATRIX_CENTER {110, 21}

uint8_t highlight_layer_keys = 0;

#if 1
// from https://github.com/74k1/lotus58_firmware/blob/main/rgb.c
led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        { 5,  4,  3,  2,  1,  0},
        { 6,  7,  8,  9, 10, 11},
        {17, 16, 15, 14, 13, 12},
        {18, 19, 20, 21, 22, 23},
    {NO_LED, 28, 27, 26, 25, 24},

                                        {34, 33, 32, 31, 30, 29},
                                        {35, 36, 37, 38, 39, 40},
                                        {46, 45, 44, 43, 42, 41},
                                        {47, 48, 49, 50, 51, 52},
                                {NO_LED, 57, 56, 55, 54, 53}
    }, {
        // LED Index to Physical Position
        // The default expected range of values for `{ x, y }` is the inclusive range `{ 0..224, 0..64 }`
        // 224 / (14 real cols + 6 cols as buffer) ~= 11 per col
        // 64 / (5 real rows + 1 row as buffer) ~= 10 per row

        // Left half
        { 55,  2}, { 44,  1}, { 33,  0}, { 22,  1}, { 11,  3}, {  0,  4},
        {  0, 14}, { 11, 13}, { 22, 11}, { 33, 10}, { 44, 11}, { 55, 12},
        { 55, 22}, { 44, 21}, { 33, 20}, { 22, 21}, { 11, 23}, {  0, 24},
        {  0, 34}, { 11, 33}, { 22, 31}, { 33, 30}, { 44, 31}, { 55, 32},
        { 70, 27}, { 66, 40}, { 50, 42}, { 39, 41} ,{ 28, 40}, // bottom row, right to left, far outer right key is first

        // Right half
        {165, 2}, {176, 1}, {187, 0}, {198, 1}, {209, 3}, {220, 4},
        {220, 14}, {209, 13}, {198, 11}, {187, 10}, {176, 11}, {165, 12},
        {165, 22}, {176, 21}, {187, 20}, {198, 21}, {209, 23}, {220, 24},
        {220, 34}, {209, 33}, {198, 31}, {187, 30}, {176, 31}, {165, 32},
        {150, 27}, {154, 40}, {170, 42}, {181, 41}, {192, 40}, // bottom row, left to right, far outer left key is first
    }, {
        // LED Index to Flag
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 1, 1, 4,

        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4,
        4, 4, 1, 1, 4
    }
};
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool is_game1_on = layer_state & (1 << _GAME);
    bool is_game2_on = layer_state & (1 << _GAME2);

    if (highlight_layer_keys)
    {
        const uint8_t layer = get_highest_layer(layer_state);
        if (layer > _GAME2 && layer >= highlight_layer_keys) {

            for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
                for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                    uint8_t index = g_led_config.matrix_co[row][col];

                    if (index >= led_min && index < led_max && index != NO_LED &&
                    keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                        rgb_matrix_set_color(index, RGB_GREEN);
                    }
                }
            }
        }
    }
    
    if (is_game1_on || is_game2_on) {
        uint8_t wsad1[] = {8, 14, 15, 16};
        uint8_t wsad2[] = {9, 13, 14, 15};
        uint8_t * wsad = is_game1_on ? wsad1 : wsad2;
        for (uint8_t i = 0; i < 4; i++) {
            if (wsad[i] >= led_min && wsad[i] < led_max) {
                rgb_matrix_set_color(wsad[i], RGB_RED);
            }
        }
    }

    return false;
}
#endif