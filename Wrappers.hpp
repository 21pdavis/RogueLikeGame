#pragma once

#include <memory>

#include "SDL.h"

class TextureWrapper
{
public:
	TextureWrapper(const char* imagePath);
	TextureWrapper(SDL_Color color, const char symbol);

	void initSurfaceInfo(std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface);

	SDL_Texture* getTextureRaw() const;
	const int getWidth() const;
	const int getHeight() const;

private:
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture{nullptr, SDL_DestroyTexture};
	int width{};
	int height{};
};
