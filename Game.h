// include/game.h
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "tank.h"
#include "EnemyTank.h"
#include "texture_manager.h"
#include "Explosion.h"
#include "explosion.h" // Thêm include
#include <fstream>
#include <SDL_mixer.h>

enum GameState {
    MENU,
    PLAYING,
    PAUSE_GAME,
    GAME_OVER,
    WIN
};

enum LEVEL
{
    EASY,
    MEDIUM,
    HARD,
    SUPER_HARD,
     NULL_GAME_LEVEL
};

class Game {
public:
    Game(); // Constructor
    void setGameSate(GameState status)
    {
        gameState = status;
    }
    GameState getGamesatus()
    {
        return gameState;
    }
private:
    GameState gameState; // Khai báo biến gameState là thành viên của class Game
};


class GameLevel
{
public:
    GameLevel()
    {
        gamelevel = NULL_GAME_LEVEL;
    }
    LEVEL getGameLevel()
    {
        return gamelevel;
    }
    void setGameLevel(LEVEL level)
    {
        gamelevel = level;
    }
private:
    LEVEL gamelevel;
};



void handleEvents(SDL_Event& e, bool& running, Tank& tank, SDL_Renderer* renderer,std::vector<EnemyTank> enemies,Game& game);
void update(Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions, SDL_Texture* explosionTexture1, SDL_Texture* explosionTexture2, SDL_Texture* explosionTexture3, Game& game, std::vector<std::pair<int, int>> indexOfZero, SDL_Renderer* renderer, int& score, const int& gamelevel, Mix_Chunk* explosionSound);
void render(SDL_Renderer* renderer, Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions, Game& game, bool& run, const int& score,int highscore);
void resetGame(Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions, int& score,SDL_Renderer* renderer);
void getHighScore(int& hightScore);
void setHighScore(int& score,int& hightScore);
#endif