#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include "game.h"
#include "tank.h"
#include "EnemyTank.h"
#include "texture_manager.h"
#include "explosion.h"
#include <utility>
#include <fstream>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Window* window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TOTAL_SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int imgFlag = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlag) & imgFlag)) {
        std::cout << "Khoi tao ham img that bai";
    }
    TTF_Init();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer init failed: " << Mix_GetError() << std::endl;
        return 1;
    }
    srand(static_cast<unsigned int>(time(NULL)));
    std::vector<EnemyTank> enemies;
    int score = 0;
    Tank tank = { 6 * 40, 10 * 40 };
    tank.texture = loadTexture("asset/tank.png", renderer);
    /*for (auto& enemy : enemies) {
        enemy.setTexture(loadTexture("asset/etank.png", renderer));
    }*/
    brickTexture = loadTexture("asset/brick.png", renderer);
    steelTexture = loadTexture("asset/steel.png", renderer);
    baseTexture = loadTexture("asset/base.png", renderer);
    SDL_Texture* explosionTexture1 = loadTexture("asset/explosion1.png", renderer);
    SDL_Texture* explosionTexture2 = loadTexture("asset/explosion2.png", renderer);
    SDL_Texture* explosionTexture3 = loadTexture("asset/explosion3.png", renderer);
    SDL_Texture* MenuTexture = loadTexture("asset/menu.jpg", renderer);
    std::vector<Explosion> explosions;
    std::vector<std::pair<int, int>> indexOfZero;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (map[y][x] == 0 && (y != 10 && x !=6)) {
                // Thêm tọa độ (x, y) vào vector indexOfZero
                indexOfZero.push_back({ x, y });
            }
        }
    }
    Game game;
    GameLevel level;
    int gamelevel;
    Mix_Music* backgroundMusic = Mix_LoadMUS("asset/background.mp3"); // Thay "background.mp3" bằng đường dẫn đến file nhạc của bạn
    if (!backgroundMusic) {
        std::cout << "Khong the tai nhac nen: " << Mix_GetError() << std::endl;
        return 1;
    }
    // Phát nhạc
    Mix_PlayMusic(backgroundMusic, -1);
    int hightScore = 0;

    bool running = true;
    SDL_Event e;
    while (running) {
        if (game.getGamesatus() == MENU)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT) {
                    running = false;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_RETURN)
                    {
                        level.setGameLevel(MEDIUM);
                    }
                    if (e.key.keysym.sym == SDLK_SPACE)
                    {
                        running = false;
                    }
                    if (e.key.keysym.sym == SDLK_1)
                    {
                        level.setGameLevel(EASY);
                    }
                    if (e.key.keysym.sym == SDLK_2)
                    {
                        level.setGameLevel(MEDIUM);
                    } 
                    if (e.key.keysym.sym == SDLK_3)
                    {
                        level.setGameLevel(HARD);
                    }
                    if (e.key.keysym.sym == SDLK_4)
                    {
                        level.setGameLevel(SUPER_HARD);
                    }
                }
            }
            switch (level.getGameLevel())
            {
            case EASY: gamelevel = 1; game.setGameSate(PLAYING); break;
            case MEDIUM:gamelevel = 2; game.setGameSate(PLAYING); break;
            case HARD:gamelevel = 3; game.setGameSate(PLAYING);  break;
            case SUPER_HARD:gamelevel = 4; game.setGameSate(PLAYING); break;
            }
            SDL_Color loadTextColor = { 255,255,255,255 };
            SDL_Texture* gameLoadeTexture1 = renderText("PRESS ENTER TO START PLAYING", "asset/win.ttf", loadTextColor, 18, renderer);
            SDL_Texture* gameLoadeTexture2 = renderText("PRESS SPACE TO EXIT THE GAME", "asset/win.ttf", loadTextColor, 18, renderer);
            SDL_Texture* gameLoadeTexture3 = renderText("SELECT GAME LEVEL < DEFAULT LEVEL:MEDIUM >", "asset/win.ttf", loadTextColor, 18, renderer);
            std::string textLevel = "1:EASY      2:MEDIUM      3:HARD      4:SUPER HARD";
            SDL_Texture* gameLoadeTexture4 = renderText(textLevel, "asset/win.ttf", loadTextColor, 15, renderer);

            int textWidth1, textHeight1;
            SDL_QueryTexture(gameLoadeTexture1, NULL, NULL, &textWidth1, &textHeight1);
            SDL_Rect LRect1 = { (TOTAL_SCREEN_WIDTH - textWidth1) / 2,350,textWidth1,textHeight1 };

            int textWidth2, textHeight2;
            SDL_QueryTexture(gameLoadeTexture2, NULL, NULL, &textWidth2, &textHeight2);
            SDL_Rect LRect2 = { (TOTAL_SCREEN_WIDTH - textWidth2) / 2,368,textWidth2,textHeight2 };

            int textWidth3, textHeight3;
            SDL_QueryTexture(gameLoadeTexture3, NULL, NULL, &textWidth3, &textHeight3);
            SDL_Rect LRect3 = { (TOTAL_SCREEN_WIDTH - textWidth3) / 2,310,textWidth3,textHeight3 };

            int textWidth4, textHeight4;
            SDL_QueryTexture(gameLoadeTexture4, NULL, NULL, &textWidth4, &textHeight4);
            SDL_Rect LRect4 = { (TOTAL_SCREEN_WIDTH - textWidth4) / 2,328,textWidth4,textHeight4 };

            SDL_RenderCopy(renderer, MenuTexture, NULL, NULL);
            SDL_RenderCopy(renderer, gameLoadeTexture1, NULL, &LRect1);
            SDL_RenderCopy(renderer, gameLoadeTexture2, NULL, &LRect2);
            SDL_RenderCopy(renderer, gameLoadeTexture3, NULL, &LRect3);
            SDL_RenderCopy(renderer, gameLoadeTexture4, NULL, &LRect4);
            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        }
        else {
            handleEvents(e, running, tank, renderer, enemies);
            if (game.getGamesatus() == PLAYING) {
                update(tank, enemies, explosions, explosionTexture1, explosionTexture2, explosionTexture3, game, indexOfZero, renderer, score,gamelevel);
            }
            render(renderer, tank, enemies, explosions, game, running, score,hightScore); // explosions them
            if (game.getGamesatus() == GAME_OVER)
            {
                setHighScore(score, hightScore);
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        running = false;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDLK_RETURN) // Nhấn Enter để chơi lại
                        {
                            SDL_RenderClear(renderer);
                            resetMap();
                            renderMap(renderer);
                            resetGame(tank, enemies, explosions, score, renderer);
                            render(renderer, tank, enemies, explosions, game, running, score,hightScore);
                            game.setGameSate(PLAYING); // Chỉnh sửa hàm setGameState
                        }
                        else if (e.key.keysym.sym == SDLK_SPACE) // Nhấn ESC để thoát
                        {
                            running = false;
                        }
                    }
                }
            }

        }

    }
    SDL_DestroyTexture(explosionTexture1);
    SDL_DestroyTexture(explosionTexture2);
    SDL_DestroyTexture(explosionTexture3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}