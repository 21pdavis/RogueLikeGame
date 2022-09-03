#ifndef GAME_H
#define GAME_H

#include <memory>

#include "SDL.h"

class Player;

#include "Elements.hpp"

class Game {
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
	
private:
	bool running = false;
	int cnt = 0;

	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unique_ptr<Player> player;
};

#endif
