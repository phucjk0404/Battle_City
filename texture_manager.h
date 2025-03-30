#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

extern SDL_Texture* brickTexture; 
extern SDL_Texture* steelTexture; 
extern SDL_Texture* baseTexture;
// Hàm tải ảnh từ đường dẫn path và trả về texture
SDL_Texture* renderText(const std::string& message, const std::string fontPath, SDL_Color color, int fontSize, SDL_Renderer* renderer);
// hàm render text score
void renderTextScore(const std::string& text, const std::string fontPath, int x, int y, SDL_Renderer* renderer,int texsize);
// Hàm tải font chữ từ đường dẫn fontPath và trả về texture
SDL_Texture* renderText(const std::string& message, const std::string fontPath, SDL_Color color, int fontSize, SDL_Renderer* renderer);
#endif#pragma once
