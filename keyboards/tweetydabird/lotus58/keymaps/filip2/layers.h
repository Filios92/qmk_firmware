#pragma once

#include "progmem.h"

enum layers {
    _QWERTY,
    _GAME,
    _GAME2,
    _NUM,
    _FN,
    _FN2,
    _NAV,
    _MYRALT, // My Right ALT
    _MOUSE,
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
    [_GAME]   = "#Game",
    [_GAME2]  = "#Gam2",
    [_NUM]    = "#Num ",
    [_FN]     = "#Func",
    [_FN2]    = "#Fun2",
    [_NAV]    = "#Nav ",
    [_MYRALT] = "#Alt ",
    [_SYS]    = "#Sys ",
    [_MOUSE]  = "#Mous",
};

