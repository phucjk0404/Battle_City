#include "texture_manager.h"
#include <iostream>
#include <SDL_ttf.h>



SDL_Texture* brickTexture = nullptr; 
SDL_Texture* steelTexture = nullptr; 
SDL_Texture* baseTexture = nullptr;

// Hàm tải ảnh từ đường dẫn path và trả về texture
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Không thể tải ảnh %s! Lỗi: %s", path, SDL_GetError());
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 33, 31, 32));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Không thể tạo texture từ ảnh %s! Lỗi: %s", path, SDL_GetError());
    }

    return texture;
}

// Hàm tải font chữ từ đường dẫn fontPath và trả về texture
SDL_Texture* renderText(const std::string& message, const std::string fontPath, SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font)
    {
        std::cout << "Mo font chu that bai" << std::endl;
        return nullptr;
    }
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!loadedSurface)
    {
        std::cout << "Ham Khoi tao font chu that bai" << std::endl;
        TTF_CloseFont(font);
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!texture)
    {
        std::cout << "Ham Khoi tao font chu that bai" << std::endl;
        TTF_CloseFont(font);
        SDL_FreeSurface(loadedSurface);
        return nullptr;
    }
    SDL_FreeSurface(loadedSurface);
    TTF_CloseFont(font);
    return texture;

}
// hàm render text score
void renderTextScore(const std::string& text, const std::string fontPath, int x, int y,SDL_Renderer* renderer,int texsize)
{
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), texsize);
    if (!font)
    {
        std::cout << "Ko the mo font chu" << std::endl;
        TTF_CloseFont(font);
        return ;
    }
    SDL_Color ScoreText = { 255,255,255,255 };
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, text.c_str(), ScoreText);
    if (!loadedSurface)
    {
        std::cout << "Khoi tao be mat that bai" << std::endl;
        SDL_FreeSurface(loadedSurface);
        TTF_CloseFont(font);
        return ;
    }
    SDL_Texture* ScoreTexure = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!ScoreTexure)
    {
        std::cout << "Khoi tao Texture that bai" << std::endl;
        SDL_FreeSurface(loadedSurface);
        TTF_CloseFont(font);
        return;
    }
    SDL_Rect drect = { x,y,loadedSurface->w,loadedSurface->h };
    SDL_RenderCopy(renderer, ScoreTexure, NULL, &drect);
    SDL_FreeSurface(loadedSurface);
    SDL_DestroyTexture(ScoreTexure);
    TTF_CloseFont(font);
}