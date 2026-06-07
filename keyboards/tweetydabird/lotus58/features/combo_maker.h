#define _GENERATE_ENUM(index, _, ...) index,
#define _GENERATE_COMBO(index, _, ...) const uint16_t PROGMEM combo_ ## index [] = {__VA_ARGS__};
#define _GENERATE_KEY_COMBOS(index, keycode, ...) [index] = COMBO(combo_ ## index, keycode),
enum combos { FOR_EACH_COMBO(_GENERATE_ENUM) COMBO_LENGTH };
FOR_EACH_COMBO(_GENERATE_COMBO);
combo_t key_combos[COMBO_LENGTH] = { FOR_EACH_COMBO(_GENERATE_KEY_COMBOS) };
uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!
