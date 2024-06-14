//
//  Created by Reasone
//  Date: 2024.06.14
//

#include "ticker_sdl.h"

using namespace std;

ticker_sdl::ticker_sdl (const int& framerate, const int& type)
{
    SDL_Init(SDL_INIT_TIMER);
    rate = framerate;
    tick_type = type;
    est_time = SDL_GetTicks();
}

ticker_sdl::~ticker_sdl ()
{
    SDL_Quit();
}

bool ticker_sdl::frame_feed ()
{
    if (tick() == 0)
    {
        return true;
    }
    return false;
}
Uint32 ticker_sdl::tick ()
{
    Uint32 eas_time = SDL_GetTicks();
    Uint32 result = eas_time - est_time;
    if (tick_type == 1)
    {
        est_time = eas_time;
    }
    return result % (1000 / rate);
}

