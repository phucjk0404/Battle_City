
#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "constants.h"
#include "texture_manager.h"
#include <vector>
#include "EnemyTank.h"

extern int map[MAP_HEIGHT][MAP_WIDTH]; 
extern int oldMap[MAP_HEIGHT][MAP_WIDTH];
void renderMap(SDL_Renderer* renderer);
void resetMap();
#endif