// GameOver -- determines if a hivolts game is over and the results.
// (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "victory.h"
struct World;

struct GameOver {

    GameOver();
    ~GameOver();

    VICTORY update(World& world);
};

#endif