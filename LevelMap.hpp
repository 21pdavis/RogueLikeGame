#pragma once

#include <array>
#include <memory>
#include <vector>

class GameElement;
#include "Elements.hpp"
#include "Constants.hpp"

// TODO: Separate Pixel out into a separate file
class Pixel {
public:
	Pixel(int x, int y);
	Pixel(int x, int y, std::unique_ptr<GameElement> startingElem);

	void update();
	void render();

	void addElem(std::unique_ptr<GameElement> elem);
	std::unique_ptr<GameElement> removeBySymbol(const char symbol);

	int getX() const;
	int getY() const;
private:
	// NOTE: Must consider implications of storing x and y in pixel and elements. Should it just be in pixel?
	//		 When rendering elements, should I do a check pixel (x, y) matches element (x, y) and update elem?
	int x;
	int y;

	Height maxHeight;
	
	// height-ordered stack of game element pointers
	std::vector<std::unique_ptr<GameElement>> stack;
	
	/*
		Iterates through stack until it finds an element with a lesser height, then inserts at itr + 1
		(on top of old highest height elem). If the new element = max height of stack, insert it at end

		Making a performance tradeoff on insertion to ensure that peeking the stack always returns the highest-
		height element. This will make rendering much faster at a cost
	*/
	void insertByHeight(std::unique_ptr<GameElement> elemPtr);
};

class LevelMap {
public:
	LevelMap();
	~LevelMap()=default;

	void update();
	void render();

	bool moveBySymbol(int x, int y, char symbol, std::pair<int, int> direction);

	bool movePlayable(const std::pair<int, int>& direction);

	static constexpr int MAP_WIDTH = 80;
	static constexpr int MAP_HEIGHT = 20;

private:
	int playableX;
	int playableY;
	std::array<std::array<std::unique_ptr<Pixel>, MAP_WIDTH>, MAP_HEIGHT> map;
};
