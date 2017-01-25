// Input -- accepts and acts on input in hivolts. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __INPUT_H__
#define __INPUT_H__

struct World;

class Input {
public:
    Input();
    ~Input();
    bool update(World& world);

private:
    struct InputImpl;
    static InputImpl _impl;
};

#endif