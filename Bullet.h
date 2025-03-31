// include/bullet.h
#ifndef BULLET_H
#define BULLET_H
#include <SDL.h>
#include "constants.h"

class Bullet {
public: 
    int x, y;
    int dx, dy;
    int speed = 8;
    SDL_Texture* texture;

    Bullet(int startX, int startY, int directionX, int directionY,SDL_Renderer* renderer);
};

#endif