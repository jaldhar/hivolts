// VICTORY -- An enumeration of victory states in the hivolts game.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef VICTORY_H
#define VICTORY_H

#include <cstdint>

enum class VICTORY : std::uint8_t {
    NONE = 0, WIN, LOSE, DRAW,
};

#endif
