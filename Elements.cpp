#include <memory>

#include "SDL.h"

#include "Elements.hpp"
#include "Colors.hpp"
#include "Wrappers.hpp"

// GameElement implementation
GameElement::GameElement(SDL_Renderer* ren, int x, int y, std::unique_ptr<TextureWrapper> texture)
	: renderer(ren), x(x), y(y)
{
	srcRect = { 0, 0, texture->getWidth(), texture->getHeight() };
	dstRect = { 0, 0, texture->getWidth(), texture->getHeight() };
	this->texture = std::move(texture);
}

// ImageElement implementation
ImageElement::ImageElement(const char* texturePath, SDL_Renderer* ren, int x, int y)
	: GameElement(ren, x, y, std::make_unique<TextureWrapper>(texturePath, ren)) {}

// TextElement Implementation
TextElement::TextElement(char symbol, const char* fontPath, int fontSize, SDL_Renderer* ren, int x, int y)
	: GameElement(ren, x, y, std::make_unique<TextureWrapper>(fontPath, fontSize, Colors::white, symbol, ren)), symbol(symbol) {}

// Player Implementation
Player::Player(const char symbol, SDL_Renderer* renderer, int x, int y)
	: TextElement(symbol, "Fonts/OpenSans-Regular.ttf", 24, renderer, x, y) {}

void Player::update()
{
	dstRect.x += 1;
	dstRect.y += 1;
}

void Player::render()
{
	SDL_RenderCopy(renderer, texture->getTextureRaw(), &srcRect, &dstRect);
}