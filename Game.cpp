#include "Game.hpp"
#include "Elements.hpp"
#include "Colors.hpp"
//#include "TextureWrapper.hpp"

std::unique_ptr<GameElement> player;

Game::Game() {
	init("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
}

Game::~Game() {
	clean();
}

const bool Game::isRunning() const {
	return running;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen) {
	// check return code of SDL_Init() to get 
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE);
		if (window) {
			std::cout << "Window initialized..." << std::endl;
		}
		else {
			std::cout << "Error initializing main window. Exiting..." << std::endl;
			return;
		}

		// Default is hardware accelerated, but specifying it here for clarity
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
			std::cout << "Renderer initialized..." << std::endl;
		}
		else { 
			std::cout << "Error initializing renderer. Exiting..." << std::endl;
			return;
		}

		// TTF_Init() returns 0 on success for some reason
		if (TTF_Init() == 0) {
			std::cout << "TrueType fonts library (SDL_ttf) initialized..." << std::endl;
		}
		else {
			std::cout << "Error with message '" << TTF_GetError() << "' while initializing TrueType fonts library(SDL_ttf)" << std::endl;
			return;
		}

		//player = std::make_unique<GameElement>("Textures/apple.png", renderer, 0, 0);
		//player = std::make_unique<Player>()

		std::cout << "Initialization finished. Running game..." << std::endl;
		running = true;
	}
}

//#pragma optimize( "", off)
void Game::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN: {
				const Uint8* state = SDL_GetKeyboardState(nullptr);
				if (state[SDL_SCANCODE_ESCAPE]) {
					running = false;
				} 
			}

			default:
				break;
		}
	}
}
//#pragma optimize( "", on)

void Game::update() {
	//player->update();
}

void Game::render() {
	// clear buffer and screen back to default color specified in SDL_SetRenderDrawColor
	SDL_RenderClear(renderer);

	//player->render();

	// render to the screen
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	TTF_CloseFont(defaultFont);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
 	std::cout << "Game Cleaned." << std::endl;
}

