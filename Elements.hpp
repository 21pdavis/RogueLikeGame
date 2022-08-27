#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "Game.hpp"
#include "Wrappers.hpp"

//Constants 
//Keys
enum class Keys {
	KEY_ESC = 27
};

enum class Height : int {
	GROUND = 0,
	ON_GROUND,
	IN_AIR
};

enum class Symbols : char {
	Player = 'P',
	Editor = 'E'
};

namespace SymbolSets {
	const char Playable[2] = {'P', 'E'};
}

class GameElement {
protected:
	GameElement(const char* texturePath, SDL_Renderer* ren, int x, int y);

	virtual void update() = 0;
	virtual void render() = 0;

private:
	int x;
	int y;

	SDL_Renderer* renderer;
	// SDL_Rect specifying dimensions of the image
	SDL_Rect srcRect;
	// SDL_Rect specifying location + dimensions of rendering
	SDL_Rect dstRect;
};

class ImageElement : GameElement{
protected:
	ImageElement(const char* texturePath, SDL_Renderer* ren, int x, int y);
	~ImageElement();
	virtual void update();
	virtual void render();
	
private:
	ImageWrapper* image;
};

class TextElement : GameElement {
protected:
	TextElement(char symbol, const char* fontPath, int fontSize, SDL_Renderer* ren, int x, int y);
	~TextElement();
	virtual void update();
	virtual void render();

private:
	SymbolWrapper* image;
};

class Player : TextElement {
public:
	Player(const char symbol, SDL_Renderer* ren, int x, int y);
	~Player();
	void update();
	void render();
};

#endif