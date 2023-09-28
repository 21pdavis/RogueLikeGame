#include <iostream>

#include "SDL_ttf.h"

#include "Game.hpp"
#include "Colors.hpp"
#include "Constants.hpp"

SDL_Renderer* Game::renderer = nullptr;
TTF_Font* Game::defaultFont = nullptr;

Game::Game()
{
	init("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

	// non-sdl initialization
	map = std::make_unique<LevelMap>();
}

Game::~Game()
{
	clean();
}

const bool Game::isRunning() const
{
	return running;
}

void Game::init(const char* title, int xpos, int ypos, bool fullScreen)
{
	// check return code of SDL_Init() to get 
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);

		std::cout << "Subsystems initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, fullScreen ? displayMode.w : 1280, fullScreen ? displayMode.h : 720,
		                          fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE);
		if (window)
		{
			std::cout << "Window initialized..." << std::endl;
		}
		else
		{
			std::cout << "Error initializing main window. Exiting..." << std::endl;
			return;
		}

		// Default is hardware accelerated, but specifying it here for clarity
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
			std::cout << "Renderer initialized..." << std::endl;
		}
		else
		{
			std::cout << "Error initializing renderer. Exiting..." << std::endl;
			return;
		}

		// TTF_Init() returns 0 on success for some reason
		if (TTF_Init() == 0)
		{
			std::cout << "TrueType fonts library and default font (SDL_ttf) initialized..." << std::endl;
		}
		else
		{
			std::cout << "Error with message '" << TTF_GetError() <<
				"' while initializing TrueType fonts library(SDL_ttf)" << std::endl;
			return;
		}

		//Game::defaultFont = TTF_OpenFont("Fonts\\OpenSans-Regular.ttf", Game::fontSize);
		Game::defaultFont = TTF_OpenFont("Fonts\\Cascadia.ttf", Game::fontSize);
		if (defaultFont)
		{
			std::cout << "Default font initialized..." << std::endl;
		}
		else
		{
			std::cout << "Error with message '" << TTF_GetError() << "' while initializing default font" << std::endl;
			return;
		}

		//map = std::make_unique<LevelMap>();
		std::cout << "Map Initialized..." << std::endl;

		std::cout << "Game Elements initialized..." << std::endl;

		std::cout << "Initialization finished. Running game..." << std::endl;
		running = true;
	}


}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN: {
				auto state = SDL_GetKeyboardState(nullptr);
				if (state[SDL_SCANCODE_UP])
				{
					map->movePlayable(Directions::UP);
				}
				else if (state[SDL_SCANCODE_DOWN])
				{
					map->movePlayable(Directions::DOWN);
				}
				else if (state[SDL_SCANCODE_LEFT])
				{
					map->movePlayable(Directions::LEFT);
				}
				else if (state[SDL_SCANCODE_RIGHT])
				{
					map->movePlayable(Directions::RIGHT);
				}
				else if (state[SDL_SCANCODE_ESCAPE])
				{
					running = false;
				}

			}

			default:
				break;
		}
	}
}

void Game::update()
{
	map->update();
}

void Game::render()
{
	// clear buffer and screen back to default color specified in SDL_SetRenderDrawColor
	SDL_RenderClear(renderer);

	map->render();

	// render to the screen
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	TTF_CloseFont(Game::defaultFont);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
 	std::cout << "Game Cleaned." << std::endl;
}
