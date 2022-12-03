#pragma once

#include <array>
#include <memory>
#include <vector>

class GameElement;
#include "Elements.hpp"
#include "Constants.hpp"

/*
LevelMap Mental Model

Items marked with * indicate potential room for expansion/complication of the model

- I want to preserve the stack-based rendering that I employed in the first iteration of the game.
- Elements should be assigned enumerated heights (on ground, in air, etc.) *
- The actual map should be a 2D array of stack vectors (of game element unique_ptr's)
	- ordered by height? Yes

- Question: How to deal with SDL_Rect's? Could create one for each pixel and wrap it with the GameElement
			stack in a Pixel class. But is this too memory-intensive? I mean... it won't be that much extra
			memory and it will make the interface simpler. BUT is it necessary when I could just store coords?
			But if I'm storing coords anyway, then that's still 2 ints. What's 2 more ints per pixel?? That's really
			not the scale of memory issues that I should be worried about. 8 bytes per pixel is a fine tradeoff for
			a simpler interface

- Question: What are all of the things that happen when a GameElement moves?
	- It must be std::move'd into the new Pixel's stack
	- Its x & y must be updated to match the new pixel

- Drawing to the map: I want to have some interface that allows for x, y specification of pixel coordinates that is then translated to 
  SDLRect Coordinates.
  e.g.:
  If a 3x5 map looks like this:
  XXXXX
  XXXXX
  XXXXX
  dstRect's x, y would get translated to (currentX * (width of text texture), currentY * (height of text texture))

- Future question: What about different levels (maps)? Should I plan for that now? I feel like planning for it now may lead
				   to that classic problem of solving a problem that I don't have the full details of yet, leading
				   to potential issues of wasted time. Let's implement a map in the way I've outlined above first,
				   then revisit the different level issue separately once I know more.

*/

// TODO: Separate Pixel out into a separate file
class Pixel {
public:
	Pixel(int x, int y);
	Pixel(int x, int y, std::unique_ptr<GameElement> startingElem);

	void update();
	void render();

	void addElem(std::unique_ptr<GameElement> elem);
	std::unique_ptr<GameElement> removeBySymbol(const char symbol);
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
