#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

extern SDL_Texture* brickTexture; // Khai báo biến toàn cục
extern SDL_Texture* steelTexture; // Khai báo biến toàn cục
extern SDL_Texture* baseTexture;
SDL_Texture* renderText(const std::string& message, const std::string fontPath, SDL_Color color, int fontSize, SDL_Renderer* renderer);
void renderTextScore(const std::string& text, const std::string fontPath, int x, int y, SDL_Renderer* renderer,int texsize);
#endif#pragma once
