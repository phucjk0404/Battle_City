// src/enemy_tank.cpp
#include "EnemyTank.h"
#include "Collision.h"
#include <stdlib.h>

EnemyTank::EnemyTank(int startX, int startY, SDL_Renderer* renderer) : renderer(renderer) {
    x = startX;
    y = startY;
}


void EnemyTank::moveRandomly() {
    int newX = x + dirX * speed;
    int newY = y + dirY * speed;

    if (checkCollision(newX, newY) ) {
        int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
        int randomIndex = rand() % 4;
        dirX = directions[randomIndex][0];
        dirY = directions[randomIndex][1];
    }
    else { 
        x = newX;
        y = newY;
    }

    isHidden = (map[y / TILE_SIZE][x / TILE_SIZE] == 3);
}


void EnemyTank::shoot() {
    bullets.push_back(Bullet(x + TANK_SIZE / 2 - BULLET_SIZE / 2,
        y + TANK_SIZE / 2 - BULLET_SIZE / 2,
        dirX, dirY, renderer));
}
void EnemyTank::removeBullets(std::function<bool(const Bullet&)> predicate) {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), predicate), bullets.end());
}