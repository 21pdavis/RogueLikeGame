// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

/*
TODOs:
- Passability/noclip implementation
- Interactions between elements
- Colors?
- Menu
*/

// TODO: create separate header(s) for constants/elements, etc.
#include "Elements.hpp"
#include "BoardOperations.hpp"

// backend modeling classes
class Pixel {
private:
	// vector to be treated as a stack (used for easier access to elements in middle of stack)
	vector<GameElement> elemStack;
public:
	Pixel() {}

	char getSymbol() {
		return elemStack.back().getSymbol();
	}

	void addElem(const GameElement& elem) {
		elemStack.push_back(elem);
	}

	void popElem() {
		// pop element off top of stack (end of vector)
		elemStack.erase(elemStack.end());
	}

	void removeElemAtIndex(int index) {
		if (elemStack.size() <= 2) {
			if (elemStack.size() == 2 && elemStack[elemStack.size() - 1].getSymbol() == 'E') {
				return;
			}
			else if (elemStack.size() < 2) {
				return;
			}
		}
		if (index < 0 || index >= elemStack.size()) {
			return;
		}
		elemStack.erase(elemStack.begin() + index);
	}

	void removeElem(unique_ptr<GameElement> other) {
		for (auto it = elemStack.begin(); it != elemStack.end(); it++) {
			if (*it == other) {
				elemStack.erase(it);
				break;
			}
		}
	}

	vector<unique_ptr<GameElement>>& getElemStack() {
		return elemStack;
	}

	void insertElement(Height height) {
		// TODO: deal with multiple items on same level
	}
};

class Map {
private:
	WINDOW* window;
	// 2D vector of Pixels
	vector<vector<Pixel>> board;
	// inner limits of board within box
	int height;
	int width;

	inline const Pixel& pixelAt(int y, int x) const {
		return board[y][x];
	}
public:
	Map(int height, int width, WINDOW* window)
		: height(height - 2), width(width - 2),  window(window) {
		// TODO: optimize? not really necessary unless many board objects are created, though,
		//		 as this code is only executed once
		for (int i = 0; i < height; i++) {
			board.push_back({});
			for (int j = 0; j < width; j++) {
				//Pixel p;
				//p->addElem(make_unique<Tile>(i + 1, j + 1));
				Pixel& p = board[i].emplace_back(Pixel());
				p.addElem(make_unique<Tile>(i + 1, j + 1));
			}
		}
	}
	
	void addElem(int y, int x, GameElement& elem) {
		board[y][x]->addElem(make_unique<GameElement>(elem));
	}
	
	void moveElem(shared_ptr<GameElement> elem, int y, int x, int newY, int newX) {
		// prevent out of bounds indexing of pixel array
		if (newY < height && newX < width && newY >= 0 && newX >= 0) {
			shared_ptr<Pixel> srcPixel = pixelAt(y, x);
			shared_ptr<Pixel> dstPixel = pixelAt(newY, newX);

			// remove element from source pixel
			srcPixel->removeElem(elem);
			// add element to destination pixel
			dstPixel->addElem(elem);
			// update placeable's awareness of its new position
			elem->setPosition(newY, newX);
		}
	}

	void editorRemoveAt(int y, int x) {
		// removes second-highest element from the target stack
		// precondition: there is an editor on top of the elemStack of the pixel
		//				 returned by pixelAt(y, x)
		shared_ptr<Pixel> pixel = pixelAt(y, x);
		pixel->removeElemAtIndex(pixel->getElemStack().size() - 2);
	}

	void removeHighestElementAt(int y, int x) {
		// retrieve elemStack from pixel at (y, x)
		shared_ptr<Pixel> pixel = pixelAt(y, x);
		vector<shared_ptr<GameElement>>& elemStack = pixel->getElemStack();
		// remove
		pixel->popElem();
	}

	void draw() {
		for (int i = 0; i < height; i++) { // y
			for (int j = 0; j < width; j++) { // x
				mvwprintw(window, i + 1, j + 1, "%c", board[i][j]->getSymbol());
			}
		}
		wrefresh(window);
	}
};

// class to wrap main functionality and render to the terminal
class Game {
public:
	Game() {
		running = true;

		// initialize PDCurses window
		// getmaxyx takes in references to height and variables and edits them
		getmaxyx(stdscr, height, width);
		window = newwin(height, width, 0, 0);
		board = this->makeElemBoard(height, width);

		// set options
		noecho(); // don't print typed characters
		keypad(window, true); // enable special keys (arrows, etc.)
		nodelay(window, true); // don't wait for input
		curs_set(0); // hide blinking cursor

		// draw border in window 
		box(window, 0, 0);
		wrefresh(window);
	}

	bool isRunning() {
		return running;
	}

	void render() {
		int key = wgetch(window);
		handleKey(key);

		draw();
	}
private:
	// TODO: Memory management refactor - use unique pointers where possible and where it seems logical
	int height;
	int width;
	//unique_ptr<Map> board; // pointer to board to prevent initialization before constructor call
	bool running;
	WINDOW* window;
	// 2D vector of stack-like vectors (basically a 3D vector)
	elemBoard<GameElement> board;
	// pointer to current playable element - nullptr before spawn 
	std::optional<std::pair<int, int>> playableCoords = std::nullopt;

	elemBoard<GameElement> makeElemBoard(int height, int width) {
		elemBoard<GameElement> board;
		for (int y = 0; y < height - 2; ++y) {
			for (int x = 0; x < width - 2; ++x) {
				board[y][x].emplace_back(std::make_unique<Tile>(y, x));
			}
		}
		return board;
	}

	void draw() {
		for (int i = 0; i < height; i++) { // y
			for (int j = 0; j < width; j++) { // x
				std::unique_ptr<GameElement> highestElem = std::move(board[i][j].back());
				mvwprintw(window, i + 1, j + 1, "%c", board[i][j].back()->getSymbol());
				board[i][j].push_back(std::move(highestElem));
			}
		}
		wrefresh(window);
	}
	
	void handleEditorAction(char key) {
		if (currentPlayable && currentPlayable->isEditor()) {
			switch (key) {
			case '\\':
				spawnElem(std::make_unique<VerticalWall>(currentPlayable->getY(), currentPlayable->getX()));
				break;
			case '-':
				spawnElem(std::make_unique<HorizontalWall>(currentPlayable->getY(), currentPlayable->getX()));
				break;
			case 'x':
				editorRemoveAt(currentPlayable->getY(), currentPlayable->getX());
				break;
			}
		}
	}

	void spawnElem(std::unique_ptr<GameElement> elem) {
		board[elem->getY()][elem->getX()].push_back(std::move(elem));
	}

	void spawnPlayable(std::unique_ptr<Playable> playable) {
		playableCoords->first = Playable::d_Y;
		playableCoords->second = Playable::d_X;
		spawnElem(std::move(playable));
	}

	void removeElem(int y, int x, const char symbols[], const size_t symbols_size) {
		// TODO: confirm that symbols pointer can safely be const
		const auto& elem = std::find_if(board[y][x].begin(), board[y][x].end(), [symbols, &symbols_size](char c) {
			std::find(symbols, symbols + symbols_size, c) != symbols + symbols_size; 
		});
		
		if (elem != board[y][x].end()) {
			board[y][x].erase(elem);
		}
	}
	
	void removeCurrentPlayable() {
		if (playableCoords) {
			constexpr size_t charNum = 2;
			char playableChars[charNum] = {'P', 'E'};
			removeElem(playableCoords->first, playableCoords->second, playableChars, charNum);
			playableCoords = std::nullopt;
		}
	}
	
	void moveElem(shared_ptr<GameElement> elem, int y, int x, int newY, int newX) {
		// see https://stackoverflow.com/questions/15004517/moving-elements-from-stdvector-to-another-one

		// prevent out of bounds indexing of pixel array
		if (newY < height && newX < width && newY >= 0 && newX >= 0) {
			shared_ptr<Pixel> srcPixel = pixelAt(y, x);
			shared_ptr<Pixel> dstPixel = pixelAt(newY, newX);

			// remove element from source pixel
			srcPixel->removeElem(elem);
			// add element to destination pixel
			dstPixel->addElem(elem);
			// update placeable's awareness of its new position
			elem->setPosition(newY, newX);
		}
	}

	void handleKey(int key) {
		switch (key) {
		case KEY_LEFT:
			if (currentPlayable) {
				moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY(), currentPlayable->getX() - 1);
			}
			break;
		case KEY_UP:
			if (currentPlayable) {
				moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY() - 1, currentPlayable->getX());
			}
			break;
		case KEY_RIGHT:
			if (currentPlayable) {
				moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY(), currentPlayable->getX() + 1);
			}
			break;
		case KEY_DOWN:
			if (currentPlayable) {
				moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY() + 1, currentPlayable->getX());
			}
			break;
		case 'p':
			playableCoords->first = Playable::d_Y;
			playableCoords->second = Playable::d_X;
			board[Playable::d_Y][Playable::d_X].emplace_back(std::make_unique<Player>());
			break;
		case 'e':
			spawnPlayable(std::make_unique<Editor>());
			break;
		case 'k':
			removeCurrentPlayable();
			break;
		// editor action cases cases
		case '\\': // vertical wall, using backslash instead of pipe
		case '-': // horizontal wall
		case 'x': // delete highest Placeable element
			handleEditorAction(key);
			break;
		case KEY_ESC:
			running = false;
			break;
		}
	}
};

int main() {
	initscr();
	Game game;
	// TODO: move this to a void Game::run() method (might fix some of the scoping/memory management stuff as a side effect)
	while (game.isRunning()) {
		game.render();
	}

	endwin();
}