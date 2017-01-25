// MhoAI -- moves the enemies in hivolts. (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <array>
#include <cstdint>
#include "mhoai.h"
#include "world.h"
using namespace std;

struct MhoAI::MhoAIImpl {
    MhoAIImpl();
    ~MhoAIImpl();

    bool diagonal(World& world, Moves& newMho, int8_t row, int8_t col);
} MhoAI::_impl;

MhoAI::MhoAIImpl::MhoAIImpl() = default;

MhoAI::MhoAIImpl::~MhoAIImpl() = default;

// A helper function to simplify diagonal movement.  The rules for how a mho
// should move diagonally are specified in design.txt
bool MhoAI::MhoAIImpl::diagonal(World& world, Moves& newMho, int8_t row,
int8_t col){

    array<Position, 3> possiblePositions{{
        {
            static_cast<int8_t>(newMho._newRow + row),
            static_cast<int8_t>(newMho._newColumn + col),
            ZORDER_MIDDLE
        },
        {
            newMho._newRow,
            static_cast<int8_t>(newMho._newColumn + col),
            ZORDER_MIDDLE
        },
        {
            static_cast<int8_t>(newMho._newRow + row),
            newMho._newColumn,
            ZORDER_MIDDLE
        },
    }};

    for (auto& p : possiblePositions) {
        if (world.entityAt(p._row, p._column) == -1) {
               newMho._newRow = p._row;
               newMho._newColumn = p._column;
               return true;
        }
    }

    for (auto& p : possiblePositions) {
        if (world._entities[world.entityAt(p._row, p._column)]
        .has(COMPONENT_FENCE)) {
            newMho._newRow = p._row;
            newMho._newColumn = p._column;
            return true;
        }
    }

    return false;
}

MhoAI::MhoAI() = default;

MhoAI::~MhoAI() = default;

// Mhos follow the Player according to an algorithm in design.txt.
void MhoAI::update(World& world) {
    Position player;

    world.foreach_entity([&world, &player](Entity& e, int i) -> bool {

        if (e.has(COMPONENT_PLAYER | COMPONENT_MOVEMENT | COMPONENT_POSITION)) {
            if (world._movements[i]._isMoving) {

                player._row = world._movements[i]._newRow;
                player._column = world._movements[i]._newColumn;

            } else {
                player = world._positions[i];
            }

            return false;
        }
        return true;
    });

    world.foreach_entity([&world, &player, this](Entity& e, int i) -> bool {

        if (e.has(COMPONENT_MHO | COMPONENT_MOVEMENT | COMPONENT_POSITION)) {

            auto& mho = world._positions[i];
            auto& newMho = world._movements[i];
            newMho._newRow = mho._row;
            newMho._newColumn = mho._column;

            if (mho._row < player._row) {
                if (mho._column < player._column) { // Mho NW of player
                    world._movements[i]._isMoving =
                        this->_impl.diagonal(world, newMho, 1, 1);

                } else if (mho._column > player._column) { // Mho NE of player
                    world._movements[i]._isMoving =
                        this->_impl.diagonal(world, newMho, 1, -1);

                } else {  // Mho N of player
                    newMho._newRow++;
                    world._movements[i]._isMoving = true;
                }

            } else if (mho._row > player._row) {
                if (mho._column < player._column) { // Mho SW of player
                    world._movements[i]._isMoving =
                        this->_impl.diagonal(world, newMho, -1, 1);

                } else if (mho._column > player._column) { // Mho SE of player
                    world._movements[i]._isMoving =
                        this->_impl.diagonal(world, newMho, -1, -1);

                } else  { // Mho S of player
                    newMho._newRow--;
                    world._movements[i]._isMoving = true;
                }

            } else {
                if (mho._column < player._column) { // Mho W of player
                    newMho._newColumn++;
                    world._movements[i]._isMoving = true;

                } else if (mho._column > player._column) { // Mho E of player
                    newMho._newColumn--;
                    world._movements[i]._isMoving = true;
                }
            }

            // If the mho would go off the board forget the whole thing.
            // Can't really happend because there would be a collision with
            // the perimeter fences first.
            if (newMho._newRow < 0 || newMho._newRow >= world._SZ ||
            newMho._newColumn < 0 || newMho._newColumn >= world._SZ) {
                newMho.clear();
            }
        }
        return true;
    });
}
