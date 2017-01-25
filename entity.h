// Entity: a game object in hivolts
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <cstdint>

typedef std::uint16_t MASK;

// Component types
static const MASK COMPONENT_NONE           = static_cast<MASK>(0);
static const MASK COMPONENT_PLAYER         = static_cast<MASK>(1 << 0);
static const MASK COMPONENT_MHO            = static_cast<MASK>(1 << 1);
static const MASK COMPONENT_EXPLOSION      = static_cast<MASK>(1 << 2);
static const MASK COMPONENT_FENCE          = static_cast<MASK>(1 << 3);
static const MASK COMPONENT_POSITION       = static_cast<MASK>(1 << 4);
static const MASK COMPONENT_APPEARANCE     = static_cast<MASK>(1 << 5);
static const MASK COMPONENT_MOVEMENT       = static_cast<MASK>(1 << 6);
static const MASK COMPONENT_JUMP           = static_cast<MASK>(1 << 7);
static const MASK COMPONENT_INPUT          = static_cast<MASK>(1 << 8);

// Every object in the game is an Entity.  An Entity is just a container for
// Components (defined in components.h)
struct Entity {
    MASK         _mask;

    Entity() : _mask{COMPONENT_NONE} {
    }

    // Some helper methods for managing components.
    void add(const MASK components) {
        _mask |= components;
    }

    void clear() {
        _mask = COMPONENT_NONE;
    }

    bool has(const MASK components) {
        return (_mask & components) == components;
    }

    void remove(const MASK components) {
        _mask &= ~components;
    }
};

#endif
