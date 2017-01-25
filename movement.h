// Movement -- handles queued movement in hivolts. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

struct World;

struct Movement {

    Movement();
    ~Movement();

    void update(World& world);
};

#endif