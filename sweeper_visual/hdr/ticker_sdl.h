//
//  Created by Reasone
//  Date: 2024.06.14
//

#ifndef TICKER_SDL_H
#define TIKCER_SDL_H

#include "iostream"

#include "SDL2/SDL.h"


class ticker_sdl
{
protected:
    Uint32 est_time;
    Uint32 rate;
    int tick_type;

public:
    explicit ticker_sdl (const int& framerate=60, const int& type=0);
    ~ticker_sdl ();
    [[nodiscard]] Uint32 tick ();
    bool frame_feed ();
};


#endif