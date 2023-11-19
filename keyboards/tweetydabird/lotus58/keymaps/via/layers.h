enum layers {
    _QWERTY,
    _FN_NAV,
    _MYRALT, // My Right ALT
    _SYS,
    _MAX_LAYERS
};

enum encoder_modes {
  ENCODER_VOL = 0,
  ENCODER_ARROW,
  ENCODER_PAGE,
  ENCODER_MODES_LEN
};

static const char PROGMEM layers_str[][6] = {
    [_QWERTY] = "     ",
    [_FN_NAV] = "#Nav ",
    [_MYRALT] = "#Alt ",
    [_SYS]    = "#Sys ",
};
