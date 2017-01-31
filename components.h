// Various components for hivolts game entities.
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "appearance.h"
#include "command.h"

struct Component {
    virtual ~Component() = default;
    virtual void clear()=0;
};

// Appearance -- a component that holds what a hivolts game entity looks like.
struct Appearance : public Component {
    Appearance() : _is{ APPEARANCE::EMPTY } {
    }

   virtual  ~Appearance() = default;

    void clear() {
        _is = APPEARANCE::EMPTY;
    }

    APPEARANCE _is;
};

// Explodes -- a component for hivolts game entities that can explode.
struct Explodes {
    Explodes() : _ttl{0} {
    }

    virtual ~Explodes() = default;

    void clear() {
        _ttl = 0;
    }

    std::uint8_t _ttl;
};

// Input -- a component for hivolts game entities that can accept input.
struct Inputs {
    Inputs() : _command {COMMAND::IGNORE} {
    }

    virtual ~Inputs() = default;

    void clear() {
        _command = COMMAND::IGNORE;
    }

    COMMAND _command;
};

// Jump -- a component for hivolts game entities that can jump.
struct Jump {
    Jump() : _isJumping{false} {
    }

    virtual ~Jump() = default;

    void clear() {
        _isJumping = false;
    }

    bool _isJumping;
};

// Moves -- a component for hivolts game entities that can move.
struct Moves {
    Moves() : _newRow {-1}, _newColumn{-1}, _isMoving{false} {
    }

    virtual ~Moves() = default;

    void clear() {
        _newRow = -1;
        _newColumn = -1;
        _isMoving = false;
    }

    std::int8_t  _newRow;
    std::int8_t  _newColumn;
    bool         _isMoving;
};

// Position -- a component that holds the position of a hivolts game entity
static const std::uint8_t ZORDER_BOTTOM = 0;
static const std::uint8_t ZORDER_MIDDLE = 1;
static const std::uint8_t ZORDER_TOP = 2;

struct Position {
    Position() : Position(-1, -1, ZORDER_BOTTOM) {
    }

    Position(std::int8_t row, std::int8_t column, std::uint8_t zorder) :
    _row {row}, _column{column}, _zorder(zorder) {
    }

    virtual ~Position() = default;

    bool operator<(const Position& p) const {
        if (_row < p._row) return true;
        if (_row > p._row) return false;
        if (_column < p._column) return true;
        if (_column > p._column) return false;
        // Not including z-order in sort order.
        return false;
    }

    bool operator>(const Position& p) const {
        return !operator<(p);
    }

    void clear() {
        _row = -1;
        _column = -1;
        _zorder = ZORDER_BOTTOM;
    }

    std::int8_t  _row;    // Not unsigned because value could be -1.
    std::int8_t  _column; //  "      "       "      "      "    "
    std::uint8_t _zorder;
};

#endif
