#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

class Game {
public:
	Game();
	~Game();

	const bool isRunning() const;

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);

	void handleEvents();
	void update();
	void render();
	void clean();
	
	// dynamic deleter functions for unique pointers
	struct SDL_Deleter {
		void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
		void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
		void operator()(TTF_Font* font) { TTF_CloseFont(font); }
	};

private:
	bool running = false;
	int cnt = 0;

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* defaultFont;
};

#endif
