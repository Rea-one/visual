//
//  Created by Reasone
//  Date: 2024.06.14
//

#include "ticker.h"

using namespace std;

auto ticker::tick () const
{
    auto eas_time = chrono::system_clock::now();
    auto result = eas_time - est_time;
    if (tick_type == 1)
    {
        est_time = eas_time;
    }
    return result;
}

bool ticker::frame_feed ()
{
    if (tick () > rate)
        return true;
    else
        return false;
}

ticker::ticker (const int& frame_rate, const int& type)
{
    rate = static_cast<chrono::system_clock::duration> (1000 / frame_rate);
    tick_type = type;
    est_time = chrono::system_clock::now();
}

ticker::~ticker ()
= default;