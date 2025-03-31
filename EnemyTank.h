// include/enemy_tank.h
#ifndef ENEMY_TANK_H
#define ENEMY_TANK_H

#include <vector>
#include <SDL.h>
#include "bullet.h"
#include "constants.h"
#include "Collision.h"
#include "map.h"
#include <functional>
#include <vector>


class EnemyTank {
private:
    int x, y;
    int speed = 4;
    int dirX = 0, dirY = -1;
    std::vector<Bullet> bullets;
    bool isHidden = false;
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer;
    int rotation = 0;
public:
    EnemyTank(int startX, int startY, SDL_Renderer* renderer);
    void moveRandomly();
    void shoot();
    void removeBullets(std::function<bool(const Bullet&)> predicate);

    // Getter để truy xuất các thành viên private 
    int getX() const { return x; }
    int getY() const { return y; }
    int getDirX() const { return dirX; }
    int getDirY() const { return dirY; }
    std::vector<Bullet>& getBullets()  { return bullets; }
    int getSpeed() const { return speed; }
    SDL_Texture* getTexture() { return texture; }
    bool getHidden() const { return isHidden; }
    SDL_Rect getRect() {
        return { x, y, TANK_SIZE, TANK_SIZE };
    }
    int getRotation() { return rotation; }
    void setRotation(int newRotation) {
        rotation = newRotation % 360; 
    }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setIsHidden(bool hidden) { isHidden = hidden; }
    void setDirX(int newDirX) { dirX = newDirX; }
    void setDirY(int newDirY) { dirY = newDirY; }
    void setTexture(SDL_Texture* newTexture) { texture = newTexture; }
};

extern std::vector<EnemyTank> enemies;

#endif