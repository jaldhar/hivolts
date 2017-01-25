// COMMAND -- An enumeration of possible commands in the hivolts game.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>

enum class COMMAND : std::uint8_t {
    BAD = 0, IGNORE, QUIT, REFRESH, RESIZE, JUMP, SIT,
    MOVENW, MOVEN, MOVENE, MOVEW, MOVEE, MOVESW, MOVES, MOVESE
};

#endif
