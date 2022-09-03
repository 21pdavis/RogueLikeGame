#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Game.hpp"
#include "Colors.hpp"

class TextureWrapper {
public:
	TextureWrapper(const char* filePath, SDL_Renderer* renderer);
	TextureWrapper(const char* filePath, int fontSize, SDL_Color color, const char symbol, SDL_Renderer* renderer);

	void initSurfaceInfo(SDL_Renderer* renderer, std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface);

	inline SDL_Texture* getTextureRaw() const { return texture.get(); };
	inline int getWidth() const { return width; };
	inline int getHeight() const { return height; };
	

private:
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
	int width;
	int height;
};

#endif
