// Render -- displays the hivolts user interface. (Interface)
//
// By Jaldhar h. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
// "Do What Thou Wilt." shall be the whole of the license.

#ifndef __RENDER_H__
#define __RENDER_H__

#include <string>
#include "victory.h"

struct World;

class Render {
public:
    Render();
    ~Render();

    void init(const std::string titleText);
    void update(World& world);
    bool end(VICTORY victory);

private:
    struct RenderImpl;
    static RenderImpl _impl;
};

#endif
