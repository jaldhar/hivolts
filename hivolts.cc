// hivolts.cc -- main routine for hivolts game.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include "collision.h"
#include "create.h"
#include "explosion.h"
#include "gameover.h"
#include "input.h"
#include "mhoai.h"
#include "movement.h"
#include "render.h"
#include "victory.h"
#include "world.h"
using namespace std;

static const int BOARDSIZE = 12;

// There are 90 entities altogether:
// 0     = the player
// 1-12  = the mhos
// 13-25 = up to 13 explosions
// 26-69 = the perimeter fences.
// 70-89 = the randomly placed fences.

static const int NUMENTITIES = 90;
static const int NUMMHOS = 12;
static const int NUMEXPLOSIONS = 13;
static const int NUMFENCES = 20;

// According to design.txt, it is the players turn until he has stopped jumping
// or moved. 
bool playersTurn(World& world) {
    bool isTurn = false;

    world.foreach_entity([&world, &isTurn](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_PLAYER | COMPONENT_MOVEMENT | COMPONENT_JUMP)) {
            if (world._jumps[i]._isJumping || !world._movements[i]._isMoving) {
                isTurn = true;
                return false;
            }
        }
        return true;
    });

    return isTurn;
}

// The main function sets up all the systems, initializes the world and display,
// and starts the game loop which continues until the player chooses to quit.
int main() {
    World world(BOARDSIZE, NUMENTITIES);
    Collision collisions;
    Explosion explosions;
    GameOver  gameOver;
    Input     input;
    MhoAI     mhoAI;
    Movement  movement;
    Render    render;
    VICTORY   victory;

    render.init("HiVolts");

    do {
        victory = VICTORY::NONE;
        {   // Does not need to be persistent.
            Create create;
            create.init(world, NUMMHOS, NUMEXPLOSIONS, NUMFENCES);
        }
        render.update(world);

        while (victory == VICTORY::NONE && input.update(world)) {

            if (!playersTurn(world)) {
                mhoAI.update(world);
            }

            movement.update(world);
            collisions.update(world);
            explosions.update(world);

            victory = gameOver.update(world);

            render.update(world);

        }

    } while (!render.end(victory));

    return 0;
}
