// include/tank.h
#ifndef TANK_H
#define TANK_H

#include <vector>
#include <SDL.h>
#include "bullet.h"
#include "constants.h"

class Tank {
public: 
    int x, y;
    int speed = 7;
    int dirX = 0, dirY = -1;
    std::vector<Bullet> bullets;
    bool isHidden = false;
    SDL_Texture* texture = nullptr;
    int lives = 3;
    Uint32 invincibleTime = 0;
    bool isBlinking = false;     // Biến kiểm tra xe đang nhấp nháy

    void takeDamage() {
        Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại

        if (currentTime > invincibleTime) { // Kiểm tra miễn thương
            lives--;
            invincibleTime = currentTime + 2000; // Miễn thương 2 giây
             // Bật hiệu ứng nhấp nháy
        }
    }

    // Hàm kiểm tra có render tank hay không
    bool shouldRender() {
        if (isBlinking) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime > invincibleTime) {
                isBlinking = false; // Hết thời gian miễn thương, tắt nhấp nháy
                return true;
            }
            // Nhấp nháy theo chu kỳ 200ms
            return (currentTime / 200) % 2 == 0;
        }
        return true;
    }
};

#endif