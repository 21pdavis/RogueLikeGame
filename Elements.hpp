#pragma once

#include "SDL.h"

class TextureWrapper;

#include "Height.hpp"
#include "Wrappers.hpp"

// TODO: Assign a unique ID to each GameElement to allow for proper searching and identification
// TODO: Merge GameEleent and TextureElement - graphics are text-based anyway
class GameElement
{
public:
	virtual void update();
	virtual void render();

	const int getX() const;
	const int getY() const;
	const bool isPlayable() const;
	const Height getHeight() const;
	char getSymbol();

	void setXY(int x, int y);

protected:
	GameElement(int x, int y, bool playable, Height height, char symbol, std::unique_ptr<TextureWrapper> texture);

	int x;
	int y;
	bool playable;
	Height height;
	char symbol;

	std::unique_ptr<TextureWrapper> texture;
	
	// SDL_Rect specifying dimensions of the image
	SDL_Rect srcRect;
	// SDL_Rect specifying location + dimensions of rendering
	SDL_Rect dstRect;
};

class ImageElement : public GameElement
{
protected:
	ImageElement(const char* texturePath, int x, int y);
};

class TextElement : public GameElement
{
protected:
	TextElement(int x, int y, bool playable, Height height, char symbol);
};

class Player : public TextElement
{
public:
	Player(int x, int y);
	void update();
};

class Tile : public TextElement
{
public:
	Tile(int x, int y);
};
