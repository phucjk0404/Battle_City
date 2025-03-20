#include "map.h"

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,2,0,0,0,2,0,0,0,0,0},
    {0,1,0,2,0,0,0,1,0,1,0,1,0},
    {0,1,0,0,0,0,1,1,0,1,2,1,0},
    {0,0,0,1,0,0,0,0,0,2,0,0,0},
    {0,0,0,1,0,0,2,0,0,1,0,1,2},
    {0,0,0,0,0,1,0,0,2,0,0,0,0},
    {0,1,1,1,0,0,0,2,0,0,0,1,0},
    {0,0,0,2,0,1,0,1,0,1,0,1,0},
    {2,1,0,2,0,1,0,1,0,0,0,1,0},
    {0,1,0,1,0,1,1,1,0,1,2,1,0},
    {0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,1,1,1,0,1,0,1,0},
    {0,0,1,0,0,1,3,1,0,1,1,1,0}
};

int oldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {0,0,0,2,0,0,0,2,0,0,0,0,0},
    {0,1,0,2,0,0,0,1,0,1,0,1,0},
    {0,1,0,0,0,0,1,1,0,1,2,1,0},
    {0,0,0,1,0,0,0,0,0,2,0,0,0},
    {0,0,0,1,0,0,2,0,0,1,0,1,2},
    {0,0,0,0,0,1,0,0,2,0,0,0,0},
    {0,1,1,1,0,0,0,2,0,0,0,1,0},
    {0,0,0,2,0,1,0,1,0,1,0,1,0},
    {2,1,0,2,0,1,0,1,0,0,0,1,0},
    {0,1,0,1,0,1,1,1,0,1,2,1,0},
    {0,1,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,1,1,1,0,1,0,1,0},
    {0,0,1,0,0,1,3,1,0,1,1,1,0}
};


void renderMap(SDL_Renderer* renderer) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            SDL_Rect tileRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (map[y][x] == 1 && brickTexture) {
                SDL_RenderCopy(renderer, brickTexture, nullptr, &tileRect);
            }
            else if (map[y][x] == 2 && steelTexture) {
                SDL_RenderCopy(renderer, steelTexture, nullptr, &tileRect);
            }
            else if (map[y][x] == 3 && baseTexture) {
                SDL_RenderCopy(renderer, baseTexture, nullptr, &tileRect);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }
    }
}

void resetMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map[y][x] = oldMap[y][x];
        }
    }
}