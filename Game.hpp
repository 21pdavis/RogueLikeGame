#pragma once

#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"

class Player;
class LevelMap;

#include "LevelMap.hpp"

class Game
{
public:
	Game();
	~Game();

	const bool isRunning() const;

	// dynamic deleter functions for unique pointers
	struct SDL_Deleter;

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	static constexpr int fontSize = 24;
	static SDL_Renderer* renderer;
	static TTF_Font* defaultFont;

private:
	bool running = false;
	int cnt = 0;

	SDL_Window* window;
	std::unique_ptr<LevelMap> map;
};
