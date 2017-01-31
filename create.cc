// Create -- Initializes the hivolts game world. (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <vector>
#include "create.h"
#include "world.h"
using namespace std;


struct Create::CreateImpl {
    CreateImpl();
    ~CreateImpl();

    void makeEntity(World& world, const int entity, const int row,
        const int column, const APPEARANCE appearance, const uint8_t zorder,
        const MASK extras);
    Position place(World& world);

    vector<vector<bool>> _board;
    uniform_int_distribution<int>     _d;
} Create::_impl;

Create::CreateImpl::CreateImpl() : _d(1, 12 - 2) {
    // _board will be initialized in Create::init().
}

Create::CreateImpl::~CreateImpl() = default;

void Create::CreateImpl::makeEntity(World& world, const int entity,
const int row, const int column, const APPEARANCE appearance,
const uint8_t zorder, const MASK extras) {
    world._entities[entity].clear();
    world._entities[entity].add(COMPONENT_APPEARANCE | COMPONENT_POSITION |
        extras);
    world._appearances[entity]._is = appearance;
    world._positions[entity]._row = row;
    world._positions[entity]._column = column;
    world._positions[entity]._zorder = zorder;
}

// Helper to put an entity in a random unoccupied space.
Position Create::CreateImpl::place(World& world) {
    Position pos;

    do {
        pos._row = _d(world._rnd);
        pos._column = _d(world._rnd);
    } while (_board[pos._row][pos._column]);

    _board[pos._row][pos._column] = true;
    return pos;
}

Create::Create() = default;

Create::~Create() = default;

void Create::init(World& world, const int numMhos,
const int numExplosions,const int numFences) {
    int nextEntity = 0;

    world.reset();

    // Clear out the board in case we are being called a second time.
    _impl._board.resize(12, vector<bool>(world._SZ, false));

    // Place Player.
    auto pos = _impl.place(world);
    _impl.makeEntity(world, nextEntity, pos._row, pos._column,
    APPEARANCE::PLAYER, ZORDER_BOTTOM,
    COMPONENT_PLAYER | COMPONENT_INPUT | COMPONENT_JUMP | COMPONENT_MOVEMENT);
    _impl._board[pos._row][pos._column] = true;
    ++nextEntity;

    // Place MHOs.
    for (auto i = 0; i < numMhos; ++i, ++nextEntity) {
        pos = _impl.place(world);
        _impl.makeEntity(world, nextEntity, pos._row, pos._column,
        APPEARANCE::MHO, ZORDER_BOTTOM, COMPONENT_MHO | COMPONENT_MOVEMENT);
        _impl._board[pos._row][pos._column] = true;
    }

    // Place explosions.  They are not visible until they...explode.
    for (auto i = 0; i < numExplosions; ++i, ++nextEntity) {
        world._entities[nextEntity].clear();
        world._entities[nextEntity].add(COMPONENT_EXPLOSION);
    }

    // Place perimeter fences.
    for (auto i = 0; i < world._SZ; i++) {
        for (auto j = 0; j < world._SZ; j++) {
            if (i == 0 || i == world._SZ - 1 || j == 0 || j == world._SZ - 1) {
                _impl.makeEntity(world, nextEntity, i, j,
                    APPEARANCE::FENCE, ZORDER_BOTTOM, COMPONENT_FENCE);
                _impl._board[i][j] = true;
                ++nextEntity;
            }
        }
    }

    // Place additional fences.
    for (auto i = 0; i < numFences; ++i, ++nextEntity) {
        pos = _impl.place(world);
        _impl.makeEntity(world, nextEntity, pos._row, pos._column,
            APPEARANCE::FENCE, ZORDER_BOTTOM, COMPONENT_FENCE);
        _impl._board[pos._row][pos._column] = true;
    }
}
