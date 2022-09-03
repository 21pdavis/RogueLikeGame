#include <iostream>

#include "SDL_ttf.h"
#include "SDL_image.h"

#include "Wrappers.hpp"
#include "Game.hpp"
#include "Colors.hpp"

TextureWrapper::TextureWrapper(const char* filePath, SDL_Renderer* renderer)
	: texture(nullptr, SDL_DestroyTexture)
{
	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(IMG_Load(filePath), SDL_FreeSurface);
	initSurfaceInfo(renderer, std::move(tempSurface));
}

TextureWrapper::TextureWrapper(const char* filePath, int fontSize, SDL_Color color, const char symbol, SDL_Renderer* renderer)
	: texture(nullptr, SDL_DestroyTexture)
{
	std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font(TTF_OpenFont(filePath, fontSize), TTF_CloseFont);

	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(TTF_RenderText_Solid(font.get(), &symbol, color), SDL_FreeSurface);
	initSurfaceInfo(renderer, std::move(tempSurface));
}

void TextureWrapper::initSurfaceInfo(SDL_Renderer* renderer, std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface)
{
	texture.reset(SDL_CreateTextureFromSurface(renderer, surface.get()));
	width = surface->w;
	height = surface->h;
}
