// include/tank.h
#ifndef TANK_H
#define TANK_H

#include <vector>
#include <SDL.h>
#include "bullet.h"
#include "constants.h"

class Tank {
public: // Thêm public để các thành viên có thể truy cập
    int x, y;
    int speed = 7;
    int dirX = 0, dirY = -1;
    std::vector<Bullet> bullets;
    bool isHidden = false;
    SDL_Texture* texture = nullptr;
};

#endif