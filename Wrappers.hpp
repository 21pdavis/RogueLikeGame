#ifndef WRAPPERS_H
#define WRAPPERS_H

#include "Game.hpp"

class TextureWrapper {
protected:
	std::unique_ptr<SDL_Texture, Game::SDL_Deleter> texture;
	int width;
	int height;

	virtual void initSurfaceInfo(SDL_Renderer* renderer, std::unique_ptr<SDL_Surface, Game::SDL_Deleter> surface);
};

class ImageWrapper : TextureWrapper {
public:
	ImageWrapper(const char* fileName, SDL_Renderer* renderer);
};

class SymbolWrapper : TextureWrapper {
public:
	SymbolWrapper(const char* filePath, int fontSize, SDL_Color color, const char symbol, SDL_Renderer* renderer);
private:
	std::unique_ptr<TTF_Font, Game::SDL_Deleter> font;
};

#endif
