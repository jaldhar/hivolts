// MhoAI -- moves the enemies in hivolts. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __MHOAI_H__
#define __MHOAI_H__

struct World;

class MhoAI {
public:
    MhoAI();
    ~MhoAI();
    void update(World& world);

private:
    struct MhoAIImpl;
    static MhoAIImpl _impl;
};

#endif