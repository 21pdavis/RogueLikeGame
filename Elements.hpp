#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "SDL.h"

class TextureWrapper;

#include "Wrappers.hpp"

//Constants 
enum class Symbols : char {
	Player = 'P',
	Editor = 'E'
};

namespace SymbolSets {
	const char Playable[2] = {'P', 'E'};
}

class GameElement {
protected:
	GameElement(SDL_Renderer* ren, int x, int y, std::unique_ptr<TextureWrapper> texture);

	virtual void update() = 0;
	virtual void render() = 0;

	int x;
	int y;

	std::unique_ptr<TextureWrapper> texture;
	SDL_Renderer* renderer;
	// SDL_Rect specifying dimensions of the image
	SDL_Rect srcRect;
	// SDL_Rect specifying location + dimensions of rendering
	SDL_Rect dstRect;
};

class ImageElement : protected GameElement{
protected:
	ImageElement(const char* texturePath, SDL_Renderer* ren, int x, int y);
};

class TextElement : protected GameElement {
protected:
	TextElement(char symbol, const char* fontPath, int fontSize, SDL_Renderer* ren, int x, int y);

private:
	char symbol;
};

class Player : public TextElement {
public:
	Player(const char symbol, SDL_Renderer* ren, int x, int y);
	void update();
	void render();
};

#endif