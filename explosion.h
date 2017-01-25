// Explosion -- handles display of explosions in hivolts.
// (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

struct World;

struct Explosion {

    Explosion();
    ~Explosion();

    void update(World& world);
};

#endif