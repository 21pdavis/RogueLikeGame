// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

/*
TODOs:
- Player movement
- Interactions between elements
- Colors?
- Menu
*/

// TODO: create separate header(s) for constants/elements, etc.?
#include "Elements.cpp"

// backend modeling classes
class Pixel {
private:
	// vector to be treated as a stack
	vector<shared_ptr<GameElement>> elemStack;
public:
	Pixel() {}

	char getSymbol() {
		return elemStack.back()->getSymbol();
	}

	void addElem(shared_ptr<GameElement> elem) {
		elemStack.push_back(elem);
	}

	void removeEnd() {
		// no need in searching for an element when you know you're popping off of the top
		elemStack.erase(elemStack.end());
	}

	void removeElemAtIndex(int index) {
		if (elemStack.size() <= 2) {
			if (elemStack.size() == 2 && elemStack[elemStack.size() - 1]->getSymbol() == 'E') {
				return;
			}
			else if (elemStack.size() < 2) {
				return;
			}
		}
		if (index < 0 || index >= elemStack.size()) { return; }
		elemStack.erase(elemStack.begin() + index);
	}

	void removeElem(shared_ptr<GameElement> other) {
		for (auto it = elemStack.begin(); it != elemStack.end(); it++) {
			if (*it == other) {
				elemStack.erase(it);
				break;
			}
		}
	}

	vector<shared_ptr<GameElement>>& getElemStack() {
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
	vector<vector<shared_ptr<Pixel>>> board;
	// inner limits of board within box
	int height;
	int width;

	inline const shared_ptr<Pixel> getPixelAt(int y, int x) const {
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
				shared_ptr<Pixel> p = make_shared<Pixel>();
				p->addElem(make_unique<Tile>(i + 1, j + 1));
				board[i].push_back(p);
			}
		}
	}
	
	void addElem(int y, int x, shared_ptr<GameElement> elem) {
		board[y][x]->addElem(elem);
	}
	
	void removeElem(int y, int x, shared_ptr<GameElement> elem) {
		board[y][x]->removeElem(elem);
	}

	void moveElem(shared_ptr<GameElement> elem, int y, int x, int newY, int newX) {
		// prevent out of bounds indexing of pixel array
		if (newY < height && newX < width && newY >= 0 && newX >= 0) {
			shared_ptr<Pixel> srcPixel = getPixelAt(y, x);
			shared_ptr<Pixel> dstPixel = getPixelAt(newY, newX);

			// remove element from source pixel
			srcPixel->removeElem(elem);
			// add element to destination pixel
			dstPixel->addElem(elem);
			// update elem's awareness of its new position
			elem->setPosition(newY, newX);
		}
	}

	void editorRemoveAt(int y, int x) {
		// removes second-highest element from the target stack
		// precondition: there is an editor on top of the elemStack of the pixel
		//				 returned by getPixelAt(y, x)
		shared_ptr<Pixel> pixel = getPixelAt(y, x);
		pixel->removeElemAtIndex(pixel->getElemStack().size() - 2);
	}

	void removeHighestElementAt(int y, int x) {
		// retrieve elemStack from pixel at (y, x)
		shared_ptr<Pixel> pixel = getPixelAt(y, x);
		vector<shared_ptr<GameElement>>& elemStack = pixel->getElemStack();
		// remove
		pixel->removeEnd();
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

// class to wrap other classes and rendere to the terminal
class Game {
private:
	// TODO: Memory management refactor - use unique pointers where possible and where it seems logical
	int height;
	int width;
	unique_ptr<Map> board; // pointer to board to prevent initialization before constructor call
	bool running;
	WINDOW* window;
	
	// TODO: Find more C++ey way to do this. Smart pointers should be used for their advantages
	//		 of scoped destruction - not to be set back to nullptr at convenience (see removeCurrentPlayable())
	shared_ptr<MovingElement> currentPlayable; // pointer to current playable element - nullptr before spawn event

	inline void spawnElem(shared_ptr<Placeable> elem) {
		board->addElem(elem->getY(), elem->getX(), elem);
	}

	void handleEditorAction(char key) {
		if (currentPlayable && currentPlayable->getSymbol() == 'E') {
			switch (key) {
			case '\\':
				spawnElem(make_shared<VerticalWall>(currentPlayable->getY(), currentPlayable->getX()));
				break;
			case '-':
				spawnElem(make_shared<HorizontalWall>(currentPlayable->getY(), currentPlayable->getX()));
				break;
			case 'x':
				board->editorRemoveAt(currentPlayable->getY(), currentPlayable->getX());
				break;
			}
		}
	}
	
	void spawnPlayable(int y, int x, shared_ptr<MovingElement> elem) {
		if (currentPlayable) { return; }
		
		currentPlayable = elem;
		board->addElem(y, x, currentPlayable);
	}

	void removeCurrentPlayable() {
		if (currentPlayable) {
			board->removeElem(currentPlayable->getY(), currentPlayable->getX(), currentPlayable);
			// TODO: make sure this is the right way to do this (I mean the API exposes it, so...)
			currentPlayable.reset(); // currentPlayable = nullptr
		}
	}
	void handleKey(int key) {
		switch (key) {
		case KEY_LEFT:
			if (currentPlayable) {
				board->moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY(), currentPlayable->getX() - 1);
			}
			break;
		case KEY_UP:
			if (currentPlayable) {
				board->moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY() - 1, currentPlayable->getX());
			}
			break;
		case KEY_RIGHT:
			if (currentPlayable) {
				board->moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY(), currentPlayable->getX() + 1);
			}
			break;
		case KEY_DOWN:
			if (currentPlayable) {
				board->moveElem(currentPlayable, currentPlayable->getY(), currentPlayable->getX(),
					currentPlayable->getY() + 1, currentPlayable->getX());
			}
			break;
		case ' ':
			spawnPlayable(0, 0, make_shared<Player>(0, 0, 100));
			break;
		case 'e':
			spawnPlayable(0, 0, make_shared<Editor>(0, 0));
			break;
		case 'k':
			removeCurrentPlayable();
			break;
		// editor element placement cases
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
public:
	Game() {
		running = true;

		// initialize PDCurses window
		getmaxyx(stdscr, height, width);
		window = newwin(height, width, 0, 0);
		board = make_unique<Map>(height, width, window);
		
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

		board->draw();
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