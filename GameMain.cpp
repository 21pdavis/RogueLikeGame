#include "Game.hpp"

int main(int argc, char* argv[])
{
	Game game;
	
	// TODO: Investigate frame rate discrepancy (nvidia showing 58 fps - should be exactly 60)
	double frameDelay = 1000.0 / 60.0;
	Uint64 lastFrameTime = 0;
	while (game.isRunning()) {
		Uint64 currTime = SDL_GetTicks64();
		if (currTime - lastFrameTime > frameDelay) {
			lastFrameTime = currTime;
			game.handleEvents();
			game.update();
			game.render();
		}
	}

	return 0;
}
