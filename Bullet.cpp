// src/bullet.cpp
#include "bullet.h"
#include "texture_manager.h" // Để sử dụng loadTexture

Bullet::Bullet(int startX, int startY, int directionX, int directionY, SDL_Renderer* renderer) { // Sửa đổi constructor
    x = startX;
    y = startY;
    dx = directionX;
    dy = directionY;
    texture = loadTexture("asset/bullet.png", renderer); // Load texture và gán
    if (!texture) {
        // Xử lý lỗi nếu không thể load texture
        SDL_Log("Không thể load texture viên đạn!");
    }
}