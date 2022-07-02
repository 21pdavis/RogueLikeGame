// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <memory>
#include <curses.h>

/*
TODOs:
- Player movement
- Interactions between elements
- Colors?
- Menu
*/

//using namespace std;
using std::vector;
using std::unique_ptr;
using std::make_unique;

//Constants 
//Keys
constexpr int KEY_ESC = 27;

enum class Height : int {
	GROUND = 0,
	ON_GROUND,
	IN_AIR
};

// static, nonmoving element for stationary objects
class GameElement {
protected:
	int y;
	int x;
	char symbol;
	bool passable;
	Height height;

	// constructor protected to prevent instantiation
	GameElement(int y, int x, char symbol, bool passable, Height height) 
		: y(y), x(x), symbol(symbol), passable(passable), height(height) {}
public:
	virtual int getY() { return y; }
	
	virtual int getX() { return x; }

	virtual char getSymbol() { return symbol; }
};

// nonstatic, moving element for moving objects
class MovingElement : public GameElement {
protected:
	MovingElement(int y, int x, char symbol, bool passable, Height height) 
		: GameElement(y, x, symbol, passable, height) {}

	virtual void move(int dx, int dy) {
		y += dy;
		x += dx;
	}
};

// moving, creature elements
class Creature : public MovingElement {
protected:
	static const int MAX_HP = 100;
	static const int MIN_HP = 0;
	int health;

	Creature(int y, int x, char symbol, int health) 
		: MovingElement(y, x, symbol, false, Height::ON_GROUND), health(health) {}
};

class Human : public Creature {
protected:
	Human(int y, int x, char symbol, int health) 
		: Creature(x, y, symbol, health) {}
};

class Player : public Human {
public:
	Player(int y, int x, int health) 
		: Human(y, x, 'P', health) {}
};

// structures and unmoving elements
class Tile : public GameElement {
public:
	Tile(int y, int x) 
		: GameElement(y, x, '.', true, Height::GROUND) {}
};

// backend modeling classes
class Pixel {
private:
	int y;
	int x;
	// vector to be treated as a stack
	vector<unique_ptr<GameElement>> elemStack;
public:
	Pixel(int y, int x)
		: y(y), x(x) {}

	char getSymbol() {
		return elemStack.back()->getSymbol();
	}

	void addElem(unique_ptr<GameElement> elem) {
		elemStack.push_back(std::move(elem));
	}

	void insertElement(Height height) {
		// TODO: deal with multiple items on same level
	}
};

class Map {
private:
	WINDOW* window;
	// 2D vector of Pixels
	vector<vector<unique_ptr<Pixel>>> map;
	// inner limits of map within box
	int height;
	int width;
	int minX, minY;
	int maxX, maxY;
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
				unique_ptr<Pixel> p = make_unique<Pixel>(i + 1, j + 1);
				p->addElem(make_unique<Tile>(i + 1, j + 1));
				map[i].push_back(std::move(p));
			}
		}
	}
	
	//string generateOutputString() {
	//	string outputString = "";
	//	for (vector<Pixel> row : map) {
	//		for (Pixel pixel : row) {
	//			outputString += pixel.getSymbol();
	//		}
	//		outputString += '\n';
	//	}
	//	return outputString;
	//}

	void draw() {
		//clear();
		for (int y = minY; y <= maxY; y++) {
			for (int x = minX; x <= maxX; x++) {
				int mapY = y - 1;
				int mapX = x - 1;
				mvwprintw(window, y, x, "%c", map[mapY][mapX]->getSymbol());
			}
		}
		wrefresh(window);
	}
};

// class to wrap other classes and rendere to the terminal
class Game {
private:
	int height;
	int width;
	std::unique_ptr<Map> map;
	bool running;
	WINDOW* window;
public:
	Game() {
		running = true;

		// TODO: figure out height discrepancy between terminal and window
		// initialize PDCurses window
		getmaxyx(stdscr, height, width);
		window = newwin(height, width, 0, 0);
		map = make_unique<Map>(height, width, window);
		
		// set options
		noecho(); // don't print typed characters
		keypad(window, true); // enable special keys (arrows, etc.)
		nodelay(window, true); // don't wait for input
		curs_set(0);

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
			
			break;
		case KEY_UP:
			
			break;
		case KEY_RIGHT:
			
			break;
		case KEY_DOWN:
			
			break;
		case KEY_ESC:
			running = false;
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