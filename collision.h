// Collision -- handles collisions between game objects in hivolts. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __COLLISION_H__
#define __COLLISION_H__

struct World;

struct Collision {

    Collision();
    ~Collision();

    void update(World& world);
};

#endif