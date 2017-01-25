// GameOver -- determines if a hivolts game is over and the results.
// (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include "gameover.h"
#include "world.h"
using namespace std;

GameOver::GameOver() {
}

GameOver::~GameOver() = default;

// Checks all the entities and if there is no player or no mhos (or both) left,
// the game is over.  In any case the appropriate victory status from victory.h
// is returned.
VICTORY GameOver::update(World& world) {
    bool playerDead = true, mhosDead = true;

    world.foreach_entity([&world, &playerDead](Entity& e, int) -> bool {
        if (e.has(COMPONENT_PLAYER)) {
            playerDead = false;
            return false;
        }
        return true;
    });

    world.foreach_entity([&world, &mhosDead](Entity& e, int) -> bool {
        if (e.has(COMPONENT_MHO)) {
            mhosDead = false;
            return false;
        }
        return true;
    });

    if (playerDead) {
        if (mhosDead) {
            return VICTORY::DRAW;
        }
        return VICTORY::LOSE;
    }

    if (mhosDead) {
        return VICTORY::WIN;
    }

    return VICTORY::NONE;
}