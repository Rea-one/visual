//
//  Created by Reasone
//  Date: 2024.06.14
//

#ifndef VISUAL_H
#define VISUAL_H

#include "iostream"
#include "vector"
#include "chrono"
#include "filesystem"

#include "SDL2/sdl.h"
#include "SDL2/SDL_image.h"

#include "rocket.h"
#include "ticker_sdl.h"

// 文件名称， 便于后续直接根据索引查找文件
// 游戏界面资源
const std::vector<std::string> dictionary =
    {std::to_string(0), std::to_string(1), std::to_string(2),
    std::to_string(3), std::to_string(4), std::to_string(5),
    std::to_string(6), std::to_string(7), std::to_string(8),
    "origin", "clicked", "mind", "flag"};

class visual
{
protected:
    bool generating = false;
    int vertical_size{};
    int horizon_size{};
    int unit{};
    ticker_sdl clock{};

    // 窗体大小
    int width{};
    int height{};
    // 窗体位置
    int horizon_position;
    int vertical_position;

    // 游戏窗口
    SDL_Window* canvas{};
    // 渲染器
    SDL_Renderer* drawer{};
    // 通信器
    rocket* leek{};
    // 资源库
    std::vector<SDL_Texture*> sources_renderer;

    void init (int vertical, int horizon);
    void set_shape (int vertical, int horizon);
    void draw (int vertical, int horizon, int type) const;
    void source_load (const std::string& file_path, SDL_Renderer* target, const std::vector<std::string>& indexes);
    void source_destroy ();
public:
    void talk ();
    visual ();
    ~visual ();
};

#endif