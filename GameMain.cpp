#include <iostream>
#include <chrono>
#include <thread>
#include "Game.hpp"

int main(int argc, char* argv[])
{
	Game game;
	
	// TODO: Investigate frame rate discrepancy (nvidia showing 58 fps - should be exactly 60)
	const double FPS = 60.0;
	double frameDelay = 1000.0 / FPS;
	Uint64 lastFrameTime = 0;
	while (game.isRunning()) {
		Uint64 startTime = SDL_GetTicks64();
		game.handleEvents();
		game.update();
		game.render();

		Uint64 currTime = SDL_GetTicks64();
		if (currTime < startTime + frameDelay)
		{
			//std::cout << "Sleeping for " << (int)(startTime + frameDelay - currTime) << " milliseconds" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(startTime + frameDelay - currTime)));
		}
	}

	return 0;
}
