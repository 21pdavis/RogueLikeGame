#include <iostream>

#include "SDL_ttf.h"
#include "SDL_image.h"

#include "Wrappers.hpp"
#include "Game.hpp"

TextureWrapper::TextureWrapper(const char* imagePath)
{
	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(IMG_Load(imagePath), SDL_FreeSurface);
	initSurfaceInfo(std::move(tempSurface));
}

//#pragma optimize("", off)
TextureWrapper::TextureWrapper(SDL_Color color, const char symbol)
{
	// init temp tempFont with auto-cleanup
	//std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> tempFont(TTF_OpenFont(fontPath, fontSize), TTF_CloseFont);
	// init temp surface with auto-cleanup
	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tempSurface(TTF_RenderText_Solid(Game::defaultFont, &symbol, color), SDL_FreeSurface);

	initSurfaceInfo(std::move(tempSurface));
}

// turn off optimization for this function
void TextureWrapper::initSurfaceInfo(std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface)
{
	texture.reset(SDL_CreateTextureFromSurface(Game::renderer, surface.get()));
	width = surface->w;
	height = surface->h;
}
//#pragma optimize("", on)

SDL_Texture* TextureWrapper::getTextureRaw() const
{
	return texture.get();
}

const int TextureWrapper::getWidth() const
{
	return width;
}

const int TextureWrapper::getHeight() const
{
	return height;
}
