#include "Wrappers.hpp"

void TextureWrapper::initSurfaceInfo(SDL_Renderer* renderer, std::unique_ptr<SDL_Surface, Game::SDL_Deleter> surface) {
	texture.reset(SDL_CreateTextureFromSurface(renderer, surface.get()));
	width = surface->w;
	height = surface->h;
}

ImageWrapper::ImageWrapper(const char* filePath, SDL_Renderer* renderer) {
	std::unique_ptr<SDL_Surface, Game::SDL_Deleter> tempSurface(IMG_Load(filePath));
	initSurfaceInfo(renderer, std::move(tempSurface));
}

SymbolWrapper::SymbolWrapper(const char* filePath, int fontSize, SDL_Color color, const char symbol, SDL_Renderer* renderer) {
	font.reset(TTF_OpenFont(filePath, fontSize));

	std::unique_ptr<SDL_Surface, Game::SDL_Deleter> tempSurface(TTF_RenderText_Solid(font.get(), &symbol, color));
	initSurfaceInfo(renderer, std::move(tempSurface));
}

