enum layers {
    _QWERTY,
    _GAME,
    _NUM,
    _FN,
    _FN2,
    _NAV,
    _MYRALT, // My Right ALT
    _SYS,
    _MOUSE,
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
    [_GAME]   = "#Game",
    [_NUM]    = "#Num ",
    [_FN]     = "#Func",
    [_FN2]    = "#Fun2",
    [_NAV]    = "#Nav ",
    [_MYRALT] = "#Alt ",
    [_SYS]    = "#Sys ",
    [_MOUSE]  = "#Mous",
};

