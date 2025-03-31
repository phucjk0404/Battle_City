#include "game.h"
#include <iostream>
#include <algorithm>
#include "Explosion.h"
#include "map.h"
#include <utility>
#include "EnemyTank.h"
#include <cstdlib> // Để sử dụng rand() và srand()
#include <ctime>
#include <string>
#include <SDL_mixer.h>


Game::Game() : gameState(MENU) {}

// Hàm di chuyển xe tăng
void handleEvents(SDL_Event& e, bool& running, Tank& tank, SDL_Renderer* renderer,std::vector<EnemyTank> enemies,Game& game) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        if (e.type == SDL_KEYDOWN) {
            int newX = tank.x, newY = tank.y;

            switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                newX -= tank.speed;
                tank.dirX = -1; tank.dirY = 0;
                break;  
            case SDLK_RIGHT:
                newX += tank.speed;
                tank.dirX = 1; tank.dirY = 0;
                break;
            case SDLK_UP:
                newY -= tank.speed;
                tank.dirX = 0; tank.dirY = -1;
                break;
            case SDLK_DOWN:
                newY += tank.speed;
                tank.dirX = 0; tank.dirY = 1;
                break;
            case SDLK_SPACE:
                tank.bullets.push_back(Bullet(
                    tank.x + TANK_SIZE / 2 - BULLET_SIZE / 2,
                    tank.y + TANK_SIZE / 2 - BULLET_SIZE / 2,
                    tank.dirX, tank.dirY, renderer
                ));
                break;
            case SDLK_p:
                if (game.getGamesatus() != PAUSE_GAME)
                {
                    game.setGameSate(PAUSE_GAME);
                }
                
            }

            if (!checkCollision(newX, newY) && !checkCollsionTank1(newX,newY,enemies)) {
                tank.x = newX;
                tank.y = newY;
            }
            if (map[newY / TILE_SIZE][newX / TILE_SIZE] == 3) {
                tank.isHidden = true;
            }
            else {
                tank.isHidden = false;
            }
        }
    }
}

// Hàm cập nhật trạng thái của game
void update(Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions, SDL_Texture* explosionTexture1, SDL_Texture* explosionTexture2, SDL_Texture* explosionTexture3,Game& game, std::vector<std::pair<int, int>> indexOfZero,SDL_Renderer* renderer,int& score, const int& gameLevel, Mix_Chunk* explosionSound, Mix_Chunk* goverSound) {
    
    // Cập nhật vị trí đạn của người chơi
    for (auto& bullet : tank.bullets) {
        bullet.x += bullet.dx * bullet.speed;
        bullet.y += bullet.dy * bullet.speed;
    }

    // Xóa đạn nếu nó ra khỏi màn hình hoặc chạm vào tường
    tank.bullets.erase(std::remove_if(tank.bullets.begin(), tank.bullets.end(), [&](Bullet& b) {
        int tileX = b.x / TILE_SIZE;
        int tileY = b.y / TILE_SIZE;

        if (b.x < 0 || b.x >= SCREEN_WIDTH || b.y < 0 || b.y >= SCREEN_HEIGHT ||
            map[tileY][tileX] == 1 || map[tileY][tileX] == 2 || map[tileY][tileX] == 3) {

            if (map[tileY][tileX] == 1) { // Nếu là gạch, thì phá hủy
                map[tileY][tileX] = 0;
            }
            return true; // Xóa viên đạn
        }
        // Kiểm tra nếu đạn bắn trúng xe tăng địch
        for (auto& enemy : enemies) {
            if (!enemy.getHidden() && b.x >= enemy.getX() && b.x <= enemy.getX() + TANK_SIZE &&
                b.y >= enemy.getY() && b.y <= enemy.getY() + TANK_SIZE) {

                // Xe tăng địch bị bắn trúng -> tạo vụ nổ
                Explosion explosion;
                explosion.x = enemy.getX();
                explosion.y = enemy.getY();
                explosion.textures[0] = explosionTexture1;
                explosion.textures[1] = explosionTexture2;
                explosion.textures[2] = explosionTexture3;
                explosion.currentTextureIndex = 0; // Bắt đầu với texture đầu tiên
                explosion.duration = 0;
                explosion.finished = false;
                explosions.push_back(explosion); // explosions là một tham số truyền vào
				Mix_PlayChannel(-1, explosionSound, 0);
                enemy.setIsHidden(true);
                score++;
                return true; // Xóa viên đạn
            }
        }
        return false;
        }), tank.bullets.end());

    // Kiểm tra va chạm với đạn địch

    Uint32 currentTime = SDL_GetTicks();
    for (auto& enemy : enemies) {
        for (auto it = enemy.getBullets().begin(); it != enemy.getBullets().end();) {
            if (!tank.isHidden &&
                currentTime > tank.invincibleTime && // Kiểm tra miễn thương
                it->x >= tank.x && it->x <= tank.x + TANK_SIZE &&
                it->y >= tank.y && it->y <= tank.y + TANK_SIZE) {

                // Giảm mạng
                tank.lives--;

                // Bật chế độ miễn thương trong 2 giây
                tank.invincibleTime = currentTime + 2000;
                tank.isBlinking = true; // Kích hoạt nhấp nháy

                // Xóa viên đạn sau khi trúng
                it = enemy.getBullets().erase(it);

                std::cout << "Mạng còn lại: " << tank.lives << std::endl;

                if (tank.lives == 0) {
                    Mix_PlayChannel(-1, explosionSound, 0);
                    Mix_PlayChannel(-1, goverSound, 0);
                    game.setGameSate(GAME_OVER);
                    tank.isHidden = true;
                    return;
                }
            }
            else {
                ++it;
            }
        }
    }

    
    // Xóa các xe tăng địch bị ẩn (bị bắn trúng) khỏi danh sách
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank& enemy) {
        return enemy.getHidden();
        }), enemies.end());
    if (enemies.size() < gameLevel)
    {
        int randomIndex = rand() % indexOfZero.size();
        std::vector<std::pair<int, int>> indext;
        for (int i = 0; i < enemies.size(); i++)
        {
            std::pair<int, int> index = { enemies[i].getX(),enemies[i].getY() };
            indext.push_back(index);
        }
        std::pair<int, int> tankindex = { tank.x,tank.y };
        indext.push_back(tankindex);
        while (true)
        {
            bool check = true;
            for (int i = 0; i < indext.size(); i++)
            {
                if (indexOfZero[randomIndex] == indext[i])
                {
                    check = false;
                    break;
                }
            }
            if (check)
            {
                break;
            }
            else {
                randomIndex = rand() % indexOfZero.size();
            }
        }
        std::pair<int, int> randomElement = indexOfZero[randomIndex];
        enemies.push_back(EnemyTank(randomElement.first * 40, randomElement.second * 40, renderer));
    }
    // Cập nhật vị trí xe tăng địch
    for (auto& enemy : enemies) {
        if (!checkCollsionTank(enemy.getX(), enemy.getY(), &enemy, enemies)) {
            int oldX = enemy.getX();
            int oldY = enemy.getY();

            enemy.moveRandomly(); // Di chuyển

            // Nếu di chuyển xong mà bị va chạm, quay đầu lại
            if (checkCollsionTank(enemy.getX(), enemy.getY(), &enemy, enemies) || checkTankCollision(enemy,tank)) {
                enemy.setX(oldX);
                enemy.setY(oldY);
                enemy.setDirX(-enemy.getDirX());
                enemy.setDirY(-enemy.getDirY());
            }
        }

        if (rand() % 100 < 5) { // 3% cơ hội bắn đạn mỗi frame
            enemy.shoot();
        }
    }
    

    
    // Cập nhật đạn của xe tăng địch
    for (auto& enemy : enemies) {
        for (Bullet& bullet : enemy.getBullets()) {
            bullet.x += bullet.dx * bullet.speed;
            bullet.y += bullet.dy * bullet.speed;
        }
    }
    for (auto& enemy : enemies) {
        enemy.removeBullets([&](const Bullet& b) {
            int tileX = b.x / TILE_SIZE;
            int tileY = b.y / TILE_SIZE;

            // Kiểm tra xem đạn có va vào tường gạch hoặc thép không
            if (b.x < 0 || b.x >= SCREEN_WIDTH || b.y < 0 || b.y >= SCREEN_HEIGHT ||
                map[tileY][tileX] == 1 || map[tileY][tileX] == 2 || map[tileY][tileX] == 3) {

                if (map[tileY][tileX] == 1) { // Nếu là gạch, phá hủy gạch
                    map[tileY][tileX] = 0;
                }
                if (map[tileY][tileX] == 3)
                {
                    game.setGameSate(GAME_OVER);
                }
                return true; // Xóa viên đạn
            }
            return false;
            });
    }
    for (auto it = explosions.begin(); it != explosions.end(); ) { // explosions là một tham số truyền vào
        it->duration++;
        if (it->duration >= 10) {
            it->duration = 0;
            it->currentTextureIndex++;
        }
        if (it->currentTextureIndex >= 3) {
            it->finished = true;
        }
        if (it->finished) {
            it = explosions.erase(it);
        }
        else {
            ++it;
        }
    }
    if (enemies.size() == 0)
    {
        game.setGameSate(WIN);
    }
}




// Hàm vẽ chính
void render(SDL_Renderer* renderer, Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions,Game& game, bool& run,const int& score, int hightscore) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
	// Vẽ map
    renderMap(renderer);
	// Vẽ xe tăng người chơi
    tank.texture = loadTexture("asset/tank.png", renderer);
	Uint32 currentTime = SDL_GetTicks();

    if (currentTime > tank.invincibleTime) {
        tank.isBlinking = false;
    }

    // Kiểm tra có cần render hay không (hiệu ứng nhấp nháy)
    bool renderTank = true;
    if (tank.isBlinking) {
        renderTank = (currentTime / 200) % 2;  // Nhấp nháy mỗi 200ms
    }

    if (!tank.isHidden && renderTank) {
        double angle = 0;
        if (tank.dirX == 1) angle = 90;    // Quay phải
        if (tank.dirX == -1) angle = -90;  // Quay trái
        if (tank.dirY == 1) angle = 180;   // Quay xuống

        SDL_Rect tankRect = { tank.x, tank.y, TANK_SIZE, TANK_SIZE };
        SDL_Point center = { TANK_SIZE / 2, TANK_SIZE / 2 };  // Tâm xoay là chính giữa xe tăng

        SDL_RenderCopyEx(renderer, tank.texture, nullptr, &tankRect, angle, &center, SDL_FLIP_NONE);
    }


    // Vẽ xe tăng địch
    for (auto& enemy : enemies) {
        if (!enemy.getHidden()) {
            double angle = 0;
            if (enemy.getDirX() == 1) angle = -90;   // Quay phải
            if (enemy.getDirX() == -1) angle = 90; // Quay trái
            if (enemy.getDirY() == -1) angle = 180;  // Quay xuống
            SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), TANK_SIZE, TANK_SIZE};
            SDL_Point center = { TANK_SIZE / 2, TANK_SIZE / 2 };
            SDL_RenderCopyEx(renderer, enemy.getTexture(), nullptr, &enemyRect, angle, &center,SDL_FLIP_NONE);
        }
    }
    for (auto& enemy : enemies) {
        enemy.setTexture(loadTexture("asset/etank.png", renderer));
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& bullet : tank.bullets) {
        if (bullet.texture) {
            SDL_Rect bulletRect = { bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE };
            SDL_RenderCopy(renderer, bullet.texture, nullptr, &bulletRect);
        }
    }

    // Vẽ đạn của địch
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.getBullets()) {
            if (bullet.texture) {
                SDL_Rect bulletRect = { bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE };
                SDL_RenderCopy(renderer, bullet.texture, nullptr, &bulletRect);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect bulletRect = { bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE };
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }
    }
	// Vẽ vụ nổ
    for (auto& explosion : explosions) {
        SDL_Rect destRect = { explosion.x, explosion.y, TANK_SIZE, TANK_SIZE };
        SDL_RenderCopy(renderer, explosion.textures[explosion.currentTextureIndex], nullptr, &destRect);
    }
	// Vẽ man hinh game over
    if (game.getGamesatus() == GAME_OVER)
    {
        SDL_Texture* gameOverIMG = loadTexture("asset/gameover.jpg", renderer);
        SDL_Color textColor = { 255,255,255,255 };
        std::string gameScore = "YOUR SCORE : " + std::to_string(score);
        SDL_Texture* gameOverTexture = renderText(gameScore, "asset/win.ttf", textColor, 50, renderer);
        int textWidth, textHeight;
        SDL_QueryTexture(gameOverTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = { (TOTAL_SCREEN_WIDTH - textWidth) / 2,320,textWidth,textHeight }; 
        SDL_Color loadTextColor = { 255,255,255,255 };
        SDL_Texture* gameLoadeTexture1 = renderText("PRESS ENTER TO PLAY AGAIN", "asset/win.ttf", loadTextColor, 18, renderer);
        SDL_Texture* gameLoadeTexture2 = renderText("PRESS SPACE TO EXIT THE GAME", "asset/win.ttf", loadTextColor, 18, renderer);

        int textWidth1, textHeight1;
        SDL_QueryTexture(gameLoadeTexture1, NULL, NULL, &textWidth1, &textHeight1);
        SDL_Rect LRect1 = { (TOTAL_SCREEN_WIDTH - textWidth1) / 2,400,textWidth1,textHeight1 };

        int textWidth2, textHeight2;
        SDL_QueryTexture(gameLoadeTexture2, NULL, NULL, &textWidth2, &textHeight2);
        SDL_Rect LRect2 = { (TOTAL_SCREEN_WIDTH - textWidth2) / 2,430,textWidth2,textHeight2 };

        SDL_RenderCopy(renderer, gameOverIMG, NULL, NULL);
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &textRect);
        SDL_RenderCopy(renderer, gameLoadeTexture1, NULL, &LRect1);
        SDL_RenderCopy(renderer, gameLoadeTexture2, NULL, &LRect2);
        SDL_DestroyTexture(gameOverTexture);
		SDL_DestroyTexture(gameLoadeTexture1);
        SDL_DestroyTexture(gameLoadeTexture2);;
		SDL_DestroyTexture(gameOverIMG);
    }
    // Vẽ màn hình chơi chính
    if (game.getGamesatus() == PLAYING)
    {
        getHighScore(hightscore);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Màu xám nhạt
        SDL_Rect divider = { SCREEN_WIDTH, 0, 2, SCREEN_HEIGHT }; // Thanh rộng 2px
        std::string gameScore = "SCORE : " + std::to_string(score);
        std::string highestScore = "HIGHEST SCORE : " + std::to_string(hightscore);
        renderTextScore(gameScore, "asset/win.ttf", SCREEN_WIDTH + 20, 50, renderer,30); 
        renderTextScore(highestScore, "asset/win.ttf", SCREEN_WIDTH + 20, 100, renderer, 18);
        renderTextScore("HOW TO PLAY : ", "asset/win.ttf", SCREEN_WIDTH + 20, 200, renderer, 28);
		renderTextScore("USE ARROW KEYS TO MOVE", "asset/win.ttf", SCREEN_WIDTH + 20, 230, renderer, 12);
		renderTextScore("PRESS SPACE TO SHOOT", "asset/win.ttf", SCREEN_WIDTH + 20, 248, renderer, 12);
        renderTextScore("GAME RULES : " , "asset/win.ttf", SCREEN_WIDTH + 20,388, renderer,28);
        renderTextScore("1 - DESTROY  MOST  ENEMY  TANKS", "asset/win.ttf", SCREEN_WIDTH + 20, 418, renderer,12);
        renderTextScore("2 - BASE  PROTECTION", "asset/win.ttf", SCREEN_WIDTH + 20, 436, renderer,12);
        SDL_RenderFillRect(renderer, &divider);
        const int HEART_SIZE = 30;  // Kích thước mỗi trái tim
        const int START_X = SCREEN_WIDTH + 20;     // Vị trí bắt đầu vẽ (góc trên trái)
        const int START_Y = 130;
        SDL_Texture* heartTexture = loadTexture("asset/heart.png", renderer);
        for (int i = 0; i < tank.lives; i++) {
            SDL_Rect heartRect = { START_X + i * (HEART_SIZE + 5), START_Y, HEART_SIZE, HEART_SIZE };
            SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRect);
        }
        SDL_DestroyTexture(heartTexture);
    }
    // Vẽ màn hình pause game
    if (game.getGamesatus() == PAUSE_GAME)
    {
		SDL_Texture* pauseIMG = loadTexture("asset/pausegame.jpg", renderer);
        SDL_Color textColor = { 255,255,255,255 };
		std::string TextPause = "PRESS P TO CONTINUE PLAYING                     PRESS SPACE TO QUIT THE GAME";
		SDL_Texture* pauseTexture = renderText(TextPause, "asset/win.ttf", textColor, 18, renderer);
		int textWidth, textHeight;
		SDL_QueryTexture(pauseTexture, NULL, NULL, &textWidth, &textHeight);
		SDL_Rect textRect = { (TOTAL_SCREEN_WIDTH - textWidth) / 2, 400, textWidth, textHeight };
        SDL_RenderCopy(renderer, pauseIMG, NULL, NULL);
        SDL_RenderCopy(renderer, pauseTexture, NULL, &textRect);
		SDL_DestroyTexture(pauseIMG);
    }
    SDL_RenderPresent(renderer);
}

// Hàm reset lại game
void resetGame(Tank& tank, std::vector<EnemyTank>& enemies, std::vector<Explosion>& explosions, int& score,SDL_Renderer* renderer) {
    enemies.clear(); // Xóa toàn bộ xe tăng địch
    explosions.clear();
   // Xóa toàn bộ vụ nổ
    tank.x = 6 * 40; // Reset xe tăng của người chơi (tạo hàm reset() nếu cần)
    tank.y = 10 * 40;
    tank.dirX = 0;
    tank.dirY = -1;
    tank.isHidden = false;
    score = 0; // Reset điểm số
}

// Hàm lấy điểm cao nhất
void getHighScore(int& hightScore)
{
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> hightScore;
        file.close();
    }
}

// Hàm so sánh điểm số và điểm cao nhất
void setHighScore(int& score,int& hightScore)
{
    if (score > hightScore)
    {
        hightScore = score;
        std::ofstream file("highscore.txt");
        if (file.is_open())
        {
            file << hightScore;
            file.close();
        }
    }
}