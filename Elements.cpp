#include <memory>

#include "SDL.h"

#include "Elements.hpp"
#include "Colors.hpp"
#include "Game.hpp"
#include "Wrappers.hpp"
#include "Constants.hpp"

// GameElement implementation
GameElement::GameElement(int x, int y, bool playable, Height height, char symbol, std::unique_ptr<TextureWrapper> texture)
	: x(x), y(y), playable(playable), height(height), symbol(symbol) 
{
	// P is 14 and 33, let's try another letter
	this->srcRect = {0, 0, texture->getWidth(), texture->getHeight()};
	this->dstRect = {0, 0, texture->getWidth(), texture->getHeight()};
	this->texture = std::move(texture);
}

#pragma optimize("", off)
void GameElement::update()
{
	dstRect.x = texture->getWidth() * x;
	dstRect.y = texture->getHeight() * y;
}
#pragma optimize("", off)

void GameElement::render()
{
	SDL_RenderCopy(Game::renderer, texture->getTextureRaw(), &srcRect, &dstRect);
}

const int GameElement::getX() const
{
	return x;
}

const int GameElement::getY() const
{
	return y;
}

const bool GameElement::isPlayable() const
{
	return playable;
}

const Height GameElement::getHeight() const
{
	return height;
}

char GameElement::getSymbol()
{
	return symbol;
}

void GameElement::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

// ImageElement implementation
//ImageElement::ImageElement(const char* texturePath, int x, int y)
//	: GameElement(x, y, std::make_unique<TextureWrapper>(texturePath)) {}

// TextElement Implementation
TextElement::TextElement(int x, int y, bool playable, Height height, char symbol)
	: GameElement(x, y, playable, height, symbol, std::make_unique<TextureWrapper>(Colors::white, symbol)) {}

// Player Implementation
Player::Player(const int x, const int y)
	: TextElement(x, y, true, Height::PLAYER, Symbols::PLAYER) {}

void Player::update()
{
	TextElement::update();
}

// Tile Implementation
Tile::Tile(const int x, const int y)
	: TextElement(x, y, false, Height::FLOOR, Symbols::TILE) {}
