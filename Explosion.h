
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <SDL.h>

struct Explosion {
    int x, y;
    SDL_Texture* textures[3]; // Mảng 3 texture
    int currentTextureIndex;
    int duration; // Thời gian hiển thị mỗi texture, có thể điều chỉnh
    bool finished;
};

#endif#pragma once
