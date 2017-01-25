// World -- a centralized data structure containg the state of a hivolts game.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __WORLD_H__
#define __WORLD_H__

#include <algorithm>
#include <functional>
#include <random>
#include <vector>
#include "components.h"
#include "entity.h"

// World is a container for components.
//
// To add a new component type:
// 1. create it in components.h
// 2. make a vector of it in struct World.
// 3. initialize it in World() constructor.
// 4. clear it in World::reset().

struct World {
    std::vector<Entity> _entities;
    std::vector<Appearance> _appearances;
    std::vector<Explodes> _explosions;
    std::vector<Inputs> _inputs;
    std::vector<Jump> _jumps;
    std::vector<Moves> _movements;
    std::vector<Position> _positions;
    const int _N;
    const int _SZ;
    std::minstd_rand _rnd;

    World(int boardsize, int numentities) : 
        _entities(numentities),
        _appearances(numentities),
        _explosions(numentities),
        _inputs(numentities),
        _jumps(numentities),
        _movements(numentities),
        _positions(numentities),
        _N{numentities},
        _SZ{boardsize},
        _rnd{std::random_device{}()} {
    }

    int entityAt(int row, int col) const {
        auto i = std::find_if(begin(_positions), end(_positions), [&row, &col]
        (const Position& p) {
            return p._row == row && p._column == col;
        });
        return i == end(_positions) ? -1 : std::distance(begin(_positions), i);
    }

    void foreach_entity(std::function<bool(Entity& e, int i)> callback) {
        for (auto i = 0; i < _N; ++i) {
            if (!callback(_entities[i], i)) {
                break;
            }
        }
    }

    void reset() {
        for (auto i = 0; i < _N; ++i) {
            _entities[i].clear();
            _appearances[i].clear();
            _explosions[i].clear();
            _inputs[i].clear();
            _jumps[i].clear();
            _movements[i].clear();
            _positions[i].clear();
        }
    }
};

#endif
