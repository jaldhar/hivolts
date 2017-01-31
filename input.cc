// Input -- accepts and acts on input in hivolts. (Implementation)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#include <algorithm>
#include <map>
#include <random>
#include <curses.h>
#include "input.h"
#include "world.h"
using namespace std;

using COMMANDMAP = map<int, COMMAND>;

struct Input::InputImpl {
    InputImpl();
    ~InputImpl();

    void badInput(World& world, const int i);
    void badMove(World& world, const int i);
    bool move(World& world, const int row, const int col);
    void jump(World& world);

    COMMANDMAP    _commandKeys;
} Input::_impl;

Input::InputImpl::InputImpl() : _commandKeys{
    // Key bindings as per design.txt
    { 'Q',          COMMAND::MOVENW },
    { 'q',          COMMAND::MOVENW },
    { KEY_HOME,     COMMAND::MOVENW },

    { 'W',          COMMAND::MOVEN },
    { 'w',          COMMAND::MOVEN },
    { KEY_UP,       COMMAND::MOVEN },

    { 'E',          COMMAND::MOVENE },
    { 'e',          COMMAND::MOVENE },
    { KEY_PPAGE,    COMMAND::MOVENE },

    { 'A',          COMMAND::MOVEW },
    { 'a',          COMMAND::MOVEW },
    { KEY_LEFT,     COMMAND::MOVEW },

    { 'D',          COMMAND::MOVEE },
    { 'd',          COMMAND::MOVEE },
    { KEY_RIGHT,    COMMAND::MOVEE },

    { 'Z',          COMMAND::MOVESW },
    { 'z',          COMMAND::MOVESW },
    { KEY_END,      COMMAND::MOVESW },

    { 'X',          COMMAND::MOVES },
    { 'x',          COMMAND::MOVES },
    { KEY_DOWN,     COMMAND::MOVES },

    { 'C',          COMMAND::MOVESE },
    { 'c',          COMMAND::MOVESE },
    { KEY_NPAGE,    COMMAND::MOVESE },

    { 'J',          COMMAND::JUMP },
    { 'j',          COMMAND::JUMP },

    { 'S',          COMMAND::SIT },
    { 's',          COMMAND::SIT },

    { 0x1b,         COMMAND::QUIT },   // Escape key

    { 0x12,         COMMAND::REFRESH}, // CTRL-R key. Handled in Render

    { KEY_RESIZE,   COMMAND::RESIZE},  // SIGWINCH. Handled in Render
}{
}

Input::InputImpl::~InputImpl() = default;


// Called when a key that is not in the command map is pressed.
void Input::InputImpl::badInput(World& world, const int i) {
    world._movements[i]._isMoving = false;
}

// called when a move that would take the player off the board is made.  It is
// only for completeness really as a collision with border fences would happen
// first.
void Input::InputImpl::badMove(World& world, const int i) {
    world._inputs[i]._command = COMMAND::BAD;
    world._movements[i]._isMoving = false;
}

// Called when the player makes a successful move.
bool Input::InputImpl::move(World& world, const int row,
const int col) {
    bool ret = false;

    world.foreach_entity([&world, &row, &col, &ret](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_MOVEMENT | COMPONENT_PLAYER)) {

            int newRow = world._positions[i]._row + row;
            int newCol = world._positions[i]._column + col;

            if (newRow < 0 || newRow >= world._SZ ||
            newCol < 0 || newCol >= world._SZ) {
                world._movements[i].clear();
                ret = false;
            } else {
                world._movements[i]._newRow = newRow;
                world._movements[i]._newColumn = newCol;
                world._movements[i]._isMoving = true;
                world._jumps[i]._isJumping = false;
                ret =  true;
            }
            return false;
        }
        return true;
    });

    return ret;
}

// Called when the player makes a jump.  Details on jumping in design.txt.
void Input::InputImpl::jump(World& world) {
    world.foreach_entity([&world](Entity& e, int i) -> bool {
        if (e.has(COMPONENT_JUMP)) {
            uniform_int_distribution<int> d(0, world._SZ - 1);
            int newRow, newCol;

            do {
                newRow = d(world._rnd);
                newCol = d(world._rnd);
            } while (world._entities[world.entityAt(newRow, newCol)]
            .has(COMPONENT_FENCE));

            world._movements[i]._newRow = newRow;
            world._movements[i]._newColumn = newCol;
            world._movements[i]._isMoving = true;
            world._jumps[i]._isJumping = true;
            return false;
        }
        return true;
    });
}

Input::Input() = default;

Input::~Input() = default;

// gets a keystroke and dispatches it based on what command is bound to it in
// commandKeys.  It returns false if the player wishes to quit the game or true
// otherwise.
bool Input::update(World& world) {
    COMMAND command = COMMAND::BAD;
    int c;

    c = getch();
    auto it = _impl._commandKeys.find(c);
    if (it != _impl._commandKeys.end()) {
        command = it->second;
    }

    bool ret = true;
    world.foreach_entity([&world, &ret, &command, this](Entity& e, int i) -> 
    bool {

        if (e.has(COMPONENT_INPUT)) {
            world._inputs[i]._command = command;
            switch(command) {
            case COMMAND::REFRESH:
            case COMMAND::RESIZE:
            case COMMAND::IGNORE:
                break;
            case COMMAND::BAD:
                this->_impl.badInput(world, i);
                break;
            case COMMAND::QUIT:
                ret = false;
                return false;
            case COMMAND::MOVENW:
                if (!this->_impl.move(world, -1, -1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVEN:
                if (!this->_impl.move(world, -1, 0)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVENE:
                if (!this->_impl.move(world, -1, 1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVEW:
                if (!this->_impl.move(world, 0, -1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVEE:
                if (!this->_impl.move(world, 0, 1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVESW:
                if (!this->_impl.move(world, 1, -1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVES:
                if (!this->_impl.move(world, 1, 0)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::MOVESE:
                if (!this->_impl.move(world, 1, 1)) {
                    this->_impl.badMove(world, i);
                }
                break;
            case COMMAND::SIT:
                this->_impl.move(world, 0, 0);
                break;
            case COMMAND::JUMP:
                this->_impl.jump(world);
                break;
            default:
                break;
            }
            return false;
        }
        return true;
    });

    return ret;
}
