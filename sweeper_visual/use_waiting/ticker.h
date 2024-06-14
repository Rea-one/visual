//
//  Created by Reasone
//  Date: 2024.06.14
//

#ifndef TICKER_H
#define TIKCER_H

#include "iostream"
#include "chrono"

class ticker
{
protected:
    std::chrono::system_clock::duration rate{};
    int tick_type{};
    auto est_time = std::chrono::high_resolution_clock::now();

public:
    explicit ticker (const int& frame_rate=60, const int& type=0);
    ~ticker ();
    [[nodiscard]] auto tick () const;
    bool frame_feed () ;
};

#endif