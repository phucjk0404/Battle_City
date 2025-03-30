// src/bullet.cpp
#include "bullet.h"
#include "texture_manager.h" // Để sử dụng loadTexture

Bullet::Bullet(int startX, int startY, int directionX, int directionY, SDL_Renderer* renderer) { 
    x = startX;
    y = startY;
    dx = directionX;
    dy = directionY;
    texture = loadTexture("asset/bullet.png", renderer); // Load texture và gán
    if (!texture) {
        
        SDL_Log("Không thể load texture viên đạn!");
    }
}