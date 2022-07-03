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

	void removeElement(shared_ptr<GameElement> other) {
		for (auto it = elemStack.begin(); it != elemStack.end(); it++) {
			if (*it == other) {
				elemStack.erase(it);
				break;
			}
		}
	}

	void insertElement(Height height) {
		// TODO: deal with multiple items on same level
	}
};

class Map {
private:
	WINDOW* window;
	// 2D vector of Pixels
	vector<vector<shared_ptr<Pixel>>> map;
	// inner limits of map within box
	int height;
	int width;
	int minX, minY;
	int maxX, maxY;

	inline const shared_ptr<Pixel> getPixelAt(int y, int x) const {
		return map[y][x];
	}
public:
	Map(int height, int width, WINDOW* window)
		: height(height - 2), width(width - 2),  window(window) {
		// Should the mins be hardcoded like this?
		minX = 1;
		minY = 1;
		maxX = width - 2;
		maxY = height - 2;

		// TODO: optimize? not really necessary unless many map objects are created, though,
		//		 as this code is only executed once
		for (int i = 0; i < height; i++) {
			map.push_back({});
			for (int j = 0; j < width; j++) {
				shared_ptr<Pixel> p = make_shared<Pixel>();
				p->addElem(make_unique<Tile>(i + 1, j + 1));
				map[i].push_back(p);
			}
		}
	}
	
	//const vector<vector<shared_ptr<Pixel>>>& getMap() const {
	//	return map;
	//}

	void moveElem(shared_ptr<GameElement> elem, int y, int x, int newY, int newX) {
		shared_ptr<Pixel> srcPixel = getPixelAt(y, x);
		shared_ptr<Pixel> dstPixel = getPixelAt(newY, newX);
		
		// remove element from source pixel
		srcPixel->removeElement(elem);
		// add element to destination pixel
		dstPixel->addElem(elem);
		// update elem's awareness of its new position
		elem->setPosition(newY, newX);
	}

	void draw() {
		//clear();
		for (int y = minY; y <= maxY; y++) {
			for (int x = minX; x <= maxX; x++) {
				mvwprintw(window, y, x, "%c", map[y - 1][x - 1]->getSymbol());
			}
		}
		wrefresh(window);
	}

	void addElem(int y, int x, shared_ptr<GameElement> elem) {
		map[y][x]->addElem(elem);
	}
};

// class to wrap other classes and rendere to the terminal
class Game {
private:
	int height;
	int width;
	unique_ptr<Map> map;
	bool running;
	WINDOW* window;
	
	// elements
	shared_ptr<Player> player;

	void spawnPlayer(int y, int x) {
		player = make_shared<Player>(y, x, 100);
		map->addElem(y, x, player);
	}
public:
	Game() {
		running = true;

		// initialize PDCurses window
		getmaxyx(stdscr, height, width);
		window = newwin(height, width, 0, 0);
		map = make_unique<Map>(height, width, window);
		
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

	void handleKey(int key) {
		switch (key) {
		case KEY_LEFT:
			if (player) {
				map->moveElem(player, player->getY(), player->getX(), player->getY(), player->getX() - 1);
			}
			break;
		case KEY_UP:
			if (player) {
				map->moveElem(player, player->getY(), player->getX(), player->getY() - 1, player->getX());
			}
			break;
		case KEY_RIGHT:
			if (player) {
				map->moveElem(player, player->getY(), player->getX(), player->getY(), player->getX() + 1);
			}
			break;
		case KEY_DOWN:
			if (player) {
				map->moveElem(player, player->getY(), player->getX(), player->getY() + 1, player->getX());
			}
			break;
		case ' ':
			spawnPlayer(0, 0);
			break;
		case KEY_ESC:
			running = false;
		case 'x':
			
			break;
		}
	}

	void render() {
		int key = wgetch(window);
		handleKey(key);

		map->draw();
	}
};

int main() {
	initscr();
	Game game;

	while (game.isRunning()) {
		game.render();
	}

	endwin();
}