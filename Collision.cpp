#include "Collision.h"
#include "EnemyTank.h"
#include <vector>

// Kiểm tra va chạm với bản đồ
bool checkCollision(int x, int y) {
    int tileX1 = x / TILE_SIZE;
    int tileY1 = y / TILE_SIZE;

    int tileX2 = (x + TANK_SIZE - 1) / TILE_SIZE;
    int tileY2 = y / TILE_SIZE;

    int tileX3 = x / TILE_SIZE;
    int tileY3 = (y + TANK_SIZE - 1) / TILE_SIZE;

    int tileX4 = (x + TANK_SIZE - 1) / TILE_SIZE;
    int tileY4 = (y + TANK_SIZE - 1) / TILE_SIZE;

    // Kiểm tra giới hạn bản đồ
    if (tileX1 < 0 || tileX1 >= MAP_WIDTH || tileY1 < 0 || tileY1 >= MAP_HEIGHT ||
        tileX2 < 0 || tileX2 >= MAP_WIDTH || tileY2 < 0 || tileY2 >= MAP_HEIGHT ||
        tileX3 < 0 || tileX3 >= MAP_WIDTH || tileY3 < 0 || tileY3 >= MAP_HEIGHT ||
        tileX4 < 0 || tileX4 >= MAP_WIDTH || tileY4 < 0 || tileY4 >= MAP_HEIGHT) {
        return true;
    }
    if (x + TANK_SIZE > SCREEN_WIDTH || y + TANK_SIZE > SCREEN_HEIGHT || x < 0 || y < 0) {
        return true;
    }
    
    return (map[tileY1][tileX1] == 1 || map[tileY1][tileX1] == 2 || map[tileY1][tileX1] == 3 || 
        map[tileY2][tileX2] == 1 || map[tileY2][tileX2] == 2 || map[tileY2][tileX2] == 3 ||
        map[tileY3][tileX3] == 1 || map[tileY3][tileX3] == 2 || map[tileY3][tileX3] == 3 ||
        map[tileY4][tileX4] == 1 || map[tileY4][tileX4] == 2 || map[tileY4][tileX4] == 3);
}

// Kiểm tra va chạm giữa các xe tăng địch
bool checkCollsionTank(int x, int y, EnemyTank* current, const std::vector<EnemyTank>& enemies) {
    SDL_Rect newRect = { x, y, TANK_SIZE, TANK_SIZE };

    for (const auto& enemy : enemies) {
        if (&enemy != current) {  // Không kiểm tra chính nó
            SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), TANK_SIZE, TANK_SIZE };
            if (SDL_HasIntersection(&newRect, &enemyRect)) {
                return true; // Có va chạm
            }
        }
    }
    return false;
}

// Kiểm tra va chạm giữa xe tăng người chơi và các xe tăng địch
bool checkCollsionTank1(int x, int y, const std::vector<EnemyTank>& enemies)
{
    SDL_Rect newRect = { x, y, TANK_SIZE, TANK_SIZE };

    for (const auto& enemy : enemies) {
            SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), TANK_SIZE, TANK_SIZE };
            if (SDL_HasIntersection(&newRect, &enemyRect)) {
                return true; // Có va chạm
            }
    }
    return false;
}

bool checkTankCollision(const EnemyTank& enemy, const Tank& player) {
    SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), TANK_SIZE, TANK_SIZE };
    SDL_Rect playerRect = { player.x, player.y, TANK_SIZE, TANK_SIZE };

    return SDL_HasIntersection(&enemyRect, &playerRect);
}