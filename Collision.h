#ifndef COLLISION_H
#define COLLISION_H

#include "constants.h"
#include "map.h"
#include "Tank.h"
#include <vector>

class EnemyTank;
// Kiểm tra va chạm với bản đồ
bool checkCollision(int x, int y);
// Kiểm tra va chạm giữa các xe tăng địch
bool checkCollsionTank(int x, int y, EnemyTank* current, const std::vector<EnemyTank>& enemies);
// Kiểm tra va chạm giữa xe tăng người chơi và các xe tăng địch
bool checkCollsionTank1(int x, int y, const std::vector<EnemyTank>& enemies);
// Kiểm tra va chạm giữa xe tăng người chơi và xe tăng địch
bool checkTankCollision(const EnemyTank& enemy, const Tank& player);
#endif