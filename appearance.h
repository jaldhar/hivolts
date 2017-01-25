// APPEARANCE -- an enumeration of what hivolts game entities can look like.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __APPEARANCE_H__
#define __APPEARANCE_H__

#include <cstdint>

enum class APPEARANCE : std::uint8_t {
    EMPTY = 0, FENCE, PLAYER, MHO, EXPLOSION1, EXPLOSION2, EXPLOSION3
};

#endif
