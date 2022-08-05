// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

/*
TODOs:
- Passability/noclip implementation
- Stack bug where higher-height elements are being hidden
- Menu
*/

// TODO: create separate header(s) for constants/elements, etc.
#include <optional>
#include "Elements.hpp"

//using elemBoard = std::vector<std::vector<std::unique_ptr<Pixel>>>;
template <class T>
using elemBoard = std::vector<std::vector<T>>;

// backend modeling classes
class Pixel {
private:
	int y;
	int x;
	bool passable;

	// vector to be treated as a stack (used for easier access to elements in middle of stack)
	std::vector<std::unique_ptr<GameElement>> elemStack;
public:
	Pixel(int y, int x): x(x), y(y), passable(false) {}
	
	const int getX() const { return x; }

	const int getY() const { return y; }

	auto findElem(GameElement& other) {
		// return a const reference to the iterator to the first element that matches the predicate
		return std::find_if(elemStack.begin(), elemStack.end(), [&](const std::unique_ptr<GameElement>& elem) {
			return *elem == other; 
		});
	}

	auto findElemCharMatch(const char symbols[], const size_t symbols_size) {
		return std::find_if(elemStack.begin(), elemStack.end(), [&](const std::unique_ptr<GameElement>& elem) {
			return std::find(symbols, symbols + symbols_size, elem->getSymbol()) != symbols + symbols_size;
		});
	}

	void addElem(std::unique_ptr<GameElement> pElem) {
		/*
		 * Given a const reference to a GameElement, add it in its appropriate place on the stack by height.
		 * In the event of GameElements of the same height, add the newer element on top of the older one.
		 */

		// TODO revisit
		//if (!pElem->isNoClip()) {
		//	passable = false;
		//}
		pElem->setPosition(y, x);
		auto it = elemStack.rbegin();
		for (; it != elemStack.rend(); ++it) {
			if (pElem->getHeight() >= (*it)->getHeight()) {
				// reverse_iterator::base() returns element "behind" (more to the right) of itself
				elemStack.insert(it.base(), std::move(pElem));
				return;
			}
		}
		// insert at very front (iterator::begin()) if no element of lesser height is found
		elemStack.insert(it.base(), std::move(pElem));
	}

	void popElem() {
		// pop element off top of stack (end of vector)
		elemStack.erase(elemStack.end());
	}

	void removeElemAtIndex(int index) {
		if (elemStack.size() <= 2) {
			if (elemStack.size() == 2 && elemStack.back()->getSymbol() == 'E') {
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

	void removeElem(GameElement& other) {
		auto elemItr = findElem(other);
		if (elemItr != elemStack.end()) {
			elemStack.erase(elemItr);
		}
	}

	void removeElemCharMatch(const char symbols[], const size_t symbols_size) {
		auto elemItr = findElemCharMatch(symbols, symbols_size);
		if (elemItr != elemStack.end()) {
			elemStack.erase(elemItr);
		}
	}

	std::vector<std::unique_ptr<GameElement>>& getElemStack() {
		return elemStack;
	}

	GameElement& back() const {
		return *elemStack.back();
	}

	void erase(std::vector<std::unique_ptr<GameElement>>::iterator itr) {
		elemStack.erase(itr);
	}

	void insertElement(Height height) {
		// TODO: deal with multiple items on same level
	}
};

// class to wrap main functionality and render to the terminal
class Game {
public:
	Game() {
		running = true;

		// initialize PDCurses window
		// !!height and width initialized here!! getmaxyx() takes in references to height and variables and edits them
		getmaxyx(stdscr, height, width);
		window = newwin(height, width, 0, 0);

		// set options
		noecho(); // don't print typed characters
		keypad(window, true); // enable special keys (arrows, etc.)
		nodelay(window, true); // don't wait for input
		curs_set(0); // hide blinking cursor

		// draw border in window 
		box(window, 0, 0);
		wrefresh(window);

		// lay HxW tiles
		initElemBoard();
	}

	bool isRunning() {
		return running;
	}

	void run() {
		while (running) {
			int key = wgetch(window);
			handleKey(key);

			draw();
		}
	}
private:
	int height;
	int width;
	// flag for whether the game is running or not
	bool running;
	WINDOW* window;
	// 2D vector of stack-like vectors (basically a 3D vector)
	elemBoard<std::unique_ptr<Pixel>> board;
	// pointer to current playable element - nullptr before spawn 
	std::optional<std::pair<int, int>> playableCoords = std::nullopt;

	void initElemBoard() {
		for (int i = 0; i < height - 2; ++i) {
			board.push_back({});
			for (int j = 0; j < width - 2; ++j) {
				board[i].emplace_back(std::make_unique<Pixel>(i, j));
				board[i][j]->addElem(std::make_unique<Tile>(i, j));
			}
		}
	}

	void draw() {
		for (int i = 0; i < height - 2; i++) {
			for (int j = 0; j < width - 2; j++) {
				mvwprintw(window, i + 1, j + 1, "%c", board[i][j]->back().getSymbol());
			}
		}
		wrefresh(window);
	}
	
	Pixel& pixelAt(int y, int x) {
		return *board[y][x];
	}

	void handleEditorAction(char key) {
		if (playableCoords) {
			switch (key) {
			case '\\':
				spawnElem(std::make_unique <VerticalWall>(playableCoords->first, playableCoords->second));
				break;
			case '-':
				spawnElem(std::make_unique<HorizontalWall>(playableCoords->first, playableCoords->second));
				break;
			case 'x':
				Pixel& p = pixelAt(playableCoords->first, playableCoords->second);
				p.removeElemAtIndex(p.getElemStack().size() - 2);
				break;
			}
		}
	}

	inline void spawnElem(std::unique_ptr<GameElement> pElem) {
		// TODO: Consider making elems ignorant of their position
		pixelAt(pElem->getY(), pElem->getX()).addElem(std::move(pElem));
	}

	inline void spawnPlayable(std::unique_ptr<Playable> pElem) {
		// reset current playable
		removeCurrentPlayable();
		playableCoords = std::make_optional<std::pair<int, int>>(Playable::d_Y, Playable::d_X);

		// use generic element spawning function
		spawnElem(std::move(pElem));
	}

	void removeCurrentPlayable() {
		if (playableCoords) {
			constexpr size_t charNum = 2;
			char playableChars[charNum] = {'P', 'E'};
			pixelAt(playableCoords->first, playableCoords->second).removeElemCharMatch(playableChars, charNum);
			playableCoords = std::nullopt;
		}
	}
	
	void moveElem(std::pair<int, int>& origPair, const int yDelta, const int xDelta) {
		// prevent out of bounds indexing of pixel array
		int newY = origPair.first + yDelta;
		int newX = origPair.second + xDelta;

		if (newY < height && newX < width && newY >= 0 && newX >= 0) {
			Pixel& srcPixel = pixelAt(origPair.first, origPair.second);
			auto elemItr = srcPixel.findElemCharMatch(SymbolSets::Playable, 2);
			if (elemItr != srcPixel.getElemStack().end()) {
				// get reference from old pixel and erase
				std::unique_ptr<GameElement> playable = std::move(*elemItr);
				srcPixel.erase(elemItr);

				// move to new pixel
				Pixel& dstPixel = pixelAt(newY, newX);
				dstPixel.addElem(std::move(playable));

				// update origPair
				origPair.first = newY;
				origPair.second = newX;
			}
		}
	}

	void handleKey(int key) {
		switch (key) {
		case KEY_LEFT:
			if (playableCoords) {
				moveElem(playableCoords.value(), 0, -1);
			}
			break;
		case KEY_UP:
			if (playableCoords) {
				moveElem(playableCoords.value(), -1, 0);
			}
			break;
		case KEY_RIGHT:
			if (playableCoords) {
				moveElem(playableCoords.value(), 0, 1);
			}
			break;
		case KEY_DOWN:
			if (playableCoords) {
				moveElem(playableCoords.value(), 1, 0);
			}
			break;
		case 'p':
			spawnPlayable(std::make_unique<Player>(100));
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
		case static_cast<int>(Keys::KEY_ESC):
			running = false;
			break;
		}
	}
};

int main() {
	initscr();
	
	Game game;
	game.run();

	endwin();
}