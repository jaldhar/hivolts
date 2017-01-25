// Movement -- handles queued movement in hivolts. (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include "movement.h"
#include "world.h"
using namespace std;

Movement::Movement() = default;

Movement::~Movement() = default;

// Updates the positions of all entities which have moved.
void Movement::update(World& world) {
    world.foreach_entity([&world](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_MOVEMENT | COMPONENT_POSITION) &&
        world._movements[i]._isMoving) {
            world._positions[i]._row = world._movements[i]._newRow;
            world._positions[i]._column = world._movements[i]._newColumn;
            world._movements[i].clear();
        }
        return true;
    });
}