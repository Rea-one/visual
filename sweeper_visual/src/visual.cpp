//
//  Created by Reasone
//  Date: 2024.06.14
//


#include "visual.h"

using namespace std;

int main(int argc, char* argv[])
{
    visual visual_game;
    visual_game.talk();

    return 0;
}

void visual::talk()
{
    mess message;
    SDL_Event event;
    while (generating)
    {
        // 事件监听
        SDL_PollEvent(&event);
        if (leek -> read(message))
        {
            // if (message.size() == 1)
            // {
            //     if (message[0] == 128)
            //         generating = false;
            // }
            if (message.size () == 2)
            {
                init(message[0], message[1]);
            }
            else if (message.size() == 3)
            {
                draw(message[1], message[2], message[0]);
            }
            if (drawer != nullptr)
            {
                SDL_RenderPresent(drawer);
            }
        }
        SDL_Delay (clock.tick ());
    }
}

void visual::draw(int vertical, int horizon, int type) const
{
    // 生成绘制矩形
    SDL_Rect position(unit * horizon, unit * vertical, unit, unit);
    // 在矩形位置绘制图片，来自于加载的纹理
    SDL_RenderCopy(drawer, sources_renderer[type], nullptr, &position);
}

void visual::set_shape(const int& vertical, const int& horizon)
{
    vertical_size = vertical;
    horizon_size = horizon;
    unit = min(700 / vertical, 1000 / horizon);
    height = vertical_size * unit;
    width = horizon_size * unit;
}

void visual::init(const int& vertical, const int& horizon)
{
    source_destroy ();
    SDL_DestroyRenderer (drawer);
    SDL_DestroyWindow (canvas);
    set_shape(vertical, horizon);
    canvas = SDL_CreateWindow("gaming ! ! !",
        horizon_position, vertical_position, width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    drawer = SDL_CreateRenderer(canvas, -1, SDL_RENDERER_ACCELERATED);
    source_load("images", drawer, dictionary);
    SDL_RenderClear(drawer);
    // 加载初始资源
    for (int ty = 0; ty < vertical_size; ty ++)
    {
        for (int yu = 0; yu < horizon_size; yu ++)
        {
            draw(ty, yu, 9);
        }
    }
}

void visual::source_load(const string& file_path, SDL_Renderer* target, const vector<string>& indexes)
{
    for (const auto& elem: filesystem::directory_iterator(file_path))
    {
        SDL_Surface* surface = IMG_Load(elem.path().string().c_str());
        int order = 0;
        for ( ; order < dictionary.size(); order ++)
        {
            // 同步索引
            if (elem.path().stem().string() == indexes[order])
                break;
        }
        if (surface != nullptr)
        {
            // 将资源加载为渲染器纹理
            sources_renderer[order] = SDL_CreateTextureFromSurface(target, surface);
        }
    }
}

void visual::source_destroy ()
{
    for (auto& elem: sources_renderer)
    {
        SDL_DestroyTexture(elem);
    }
}

visual::visual()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    vertical_position = SDL_WINDOWPOS_CENTERED;
    horizon_position = SDL_WINDOWPOS_CENTERED;
    leek = new rocket;
    leek -> link_to();
    sources_renderer = vector<SDL_Texture*>(13, nullptr);
    generating = true;
}
visual::~visual()
{
    SDL_DestroyRenderer (drawer);
    SDL_DestroyWindow(canvas);
    delete leek;
    SDL_Quit();
}

