#ifndef COLLISION_H
#define COLLISION_H

#include "constants.h"
#include "map.h"
#include "Tank.h"
#include <vector>

class EnemyTank;
bool checkCollision(int x, int y);
bool checkCollsionTank(int x, int y, EnemyTank* current, const std::vector<EnemyTank>& enemies);
bool checkCollsionTank1(int x, int y, const std::vector<EnemyTank>& enemies);
bool checkTankCollision(const EnemyTank& enemy, const Tank& player);
#endif