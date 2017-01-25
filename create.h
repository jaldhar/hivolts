// Create -- Initializes the hivolts game world. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __CREATE_H__
#define __CREATE_H__

struct World;

class Create {
public:
    Create();
    ~Create();
    void init(World& world, const int numMhos, const int numExplosions,
    const int numFences);

private:
    struct CreateImpl;
    static CreateImpl _impl;
};

#endif