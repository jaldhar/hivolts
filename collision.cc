// Collision -- handles collisions between game objects in hivolts.
// (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <map>
#include <set>
#include "collision.h"
#include "world.h"
using namespace std;

Collision::Collision() = default;

Collision::~Collision() = default;

void Collision::update(World& world) {
    multimap<Position, int> positions;

    world.foreach_entity([&world, &positions](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_POSITION) && !e.has(COMPONENT_EXPLOSION)) {
            positions.emplace(world._positions[i], i);
        }
        return true;
    });

    set<Position> keys; // List of unique positions.
    for (auto pos: positions) {
        keys.emplace(pos.first);
    }

    for (auto key: keys) {
        if (positions.count(key) > 1) { // More than 1 entity in the same place.
            auto collisions = positions.equal_range(key);
            for (auto i = collisions.first; i != collisions.second; ++i) {
                Entity& collided  = world._entities[i->second];

                // Disappear mho or player entities that occupy the same place.
                if (collided.has(COMPONENT_PLAYER) ||
                collided.has(COMPONENT_MHO)) {
                    collided.remove(COMPONENT_PLAYER | COMPONENT_MHO |
                        COMPONENT_APPEARANCE | COMPONENT_POSITION);
                    world._appearances[i->second].clear();
                    world._positions[i->second].clear();
                }
            }

            // Replace with the first unused explosion or if there is already
            // an explosion there, re-up its' TTL.
            world.foreach_entity([&world, &key](Entity& e, int i) -> bool {

                if (e.has(COMPONENT_EXPLOSION)) {
                    Position& p = world._positions[i];

                    if (e.has(COMPONENT_POSITION) && p._row == key._row &&
                        p._column == key._column) {
                        world._explosions[i]._ttl = 3;
                        return false;

                    } else if (!e.has(COMPONENT_POSITION)) {
                        e.add(COMPONENT_APPEARANCE | COMPONENT_POSITION);
                        p._row = key._row;
                        p._column = key._column;
                        p._zorder = ZORDER_TOP;
                        world._explosions[i]._ttl = 3;
                        return false;
                    }
                }
                return true;
            });
        }
    }
}