// Explosion -- handles display of explosions in hivolts.
// (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <map>
#include <set>
#include <utility>
#include "explosion.h"
#include "world.h"
using namespace std;

Explosion::Explosion() = default;

Explosion::~Explosion() = default;

// reduce the TTL for each visible explosion and change its appearance.  When
// the TTL reaches zero, remove the explosion from sight.
void Explosion::update(World& world) {

    world.foreach_entity([&world](Entity& e, int i) -> bool {

        if (e.has(COMPONENT_EXPLOSION | COMPONENT_APPEARANCE |
        COMPONENT_POSITION)) {
            switch(world._explosions[i]._ttl) {
            case 3:
                world._appearances[i]._is = APPEARANCE::EXPLOSION1;
                world._explosions[i]._ttl--;
                break;
            case 2:
                world._appearances[i]._is = APPEARANCE::EXPLOSION2;
                world._explosions[i]._ttl--;
                break;
            case 1:
                world._appearances[i]._is = APPEARANCE::EXPLOSION3;
                world._explosions[i]._ttl--;
                break;
            case 0:
                world._appearances[i].clear();
                world._positions[i].clear();
                e.remove(COMPONENT_APPEARANCE | COMPONENT_POSITION);
                break;
            default:
                break;
            }
        }
        return true;
    });
}