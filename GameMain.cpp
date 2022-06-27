// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <memory>
#include "conio.h"
#include "GameHeader.h" // not used yet - may just delete in favor of a cpp file that contains the Element classes

//Constants 
//Keys
constexpr int LEFT = 75;
constexpr int UP = 72;
constexpr int RIGHT = 77;
constexpr int DOWN = 80;

using namespace std;
//using std::vector;
//using std::cout;
//using std::endl;
//using std::array;
//using std::vector;
//using std::stack;
//using std::unique_ptr;
//using std::make_unique;
//using std::shared_ptr;
//using std::make_shared;

enum class Height : int {
	GROUND = 0,
	ON_GROUND,
	IN_AIR
};

// static, nonmoving element for stationary objects (GameElements are static by default)
class GameElement {
protected:
	int x;
	int y;
	char symbol;
	bool passable;
	Height height;

	// constructor protected to prevent instantiation
	GameElement(int x, int y, char symbol, bool passable, Height height) 
		: x(x), y(y), symbol(symbol), passable(passable), height(height) {}
public:
	virtual int getX() { return this->x; }

	virtual int getY() { return this->y; }

	virtual char getSymbol() { return this->symbol; }
};

// nonstatic, moving element for moving objects
class MovingElement : public GameElement {
protected:
	MovingElement(int x, int y, char symbol, bool passable, Height height) : GameElement(x, y, symbol, passable, height) {}

	virtual void move(int dx, int dy) {
		this->x += dx;
		this->y += dy;
	}
};

// TODO: Add an Entity/Creature class
// TODO: Add static vs. nonstatic GameElement classes

// moving, creature elements
class Creature : public MovingElement {
protected:
	static const int MAX_HP = 100;
	static const int MIN_HP = 0;
	int health;

	Creature(int x, int y, char symbol, int health) 
		: MovingElement(x, y, symbol, false, Height::ON_GROUND), health(health) {}
};

class Human : public Creature {
protected:
	Human(int x, int y, char symbol, int health) : Creature(x, y, symbol, health) {}
};

class Player : public Human {
public:
	Player(int x, int y, int health) : Human(x, y, 'P', health) {}
};

// structures and unmoving elements
class Tile : public GameElement {
public:
	Tile(int x, int y) : GameElement(x, y, '.', true, Height::GROUND) {}
};

// backend modeling classes
// are Pixel objects unnecessary? idk maybe, but the interface they provide is nice, so I might keep it,
// and I find it helpful in visualizing what's happening
class Pixel {
private:
	// vector to be treated as a stack
	vector<shared_ptr<GameElement>> elemStack;
public:
	Pixel(shared_ptr<GameElement> elem) {
		this->elemStack.push_back(elem);
	}
	
	Pixel(const Pixel& other) {
		//for (int i = 0; i < elemStack.size(); i++) {
		//	this->elemStack.push_back(make_unique<GameElement>(*other.elemStack[i]));
		//}
		this->elemStack = other.elemStack;
	}

	// assignment operator
	Pixel& operator= (const Pixel& other) {
		this->elemStack = other.elemStack;
		return *this;
	}

	char getSymbol() {
		return this->elemStack.back()->getSymbol();
	}

	void addElem(shared_ptr<GameElement> elem) {
		this->elemStack.push_back(elem);
	}

	void insertElement(Height height) {
		// TODO: deal with multiple items on same level
	}
};

class Map {
private:
	int width;
	int height;
	// 2D vector of Pixels
	vector<vector<Pixel>> map;

	void clear() {
		for (vector<Pixel> row : map) {
			for (Pixel pixel : row) {
				cout << '\b';
			}
		}

		//for (vector<Pixel> row : map) {
		//	for (Pixel pixel : row) {
		//		cout << ' ';
		//	}
		//}
	}
public:
	Map(int width, int height) : width(width), height(height) {
		for (int i = 0; i < height; i++) {
			this->map.push_back({});
			for (int j = 0; j < width; j++) {
				this->map[i].push_back(Pixel(make_shared<Tile>(i, j)));
			}
		}
	}
	
	string generateOutputString() {
		string outputString = "";
		for (vector<Pixel> row : map) {
			for (Pixel pixel : row) {
				outputString += pixel.getSymbol();
			}
			outputString += '\n';
		}
		return outputString;
	}

	void draw() {
		this->clear();
		// make a string and edit it? idk
		for (vector<Pixel> row : map) {
			for (Pixel pixel : row) {
				cout << pixel.getSymbol();
			}
			cout << endl;
		}
	}
};

class Game {
private:
	int width;
	int height;
	unique_ptr<Map> map;
public:
	Game(int width, int height)
		: width(width), height(height), map(make_unique<Map>(width, height)) {}
	
	void render() {
		map->draw();
	}

	void handleKey(int key) {
		switch (key) {
		case LEFT:
			
			break;
		case UP:
			
			break;
		case RIGHT:
			
			break;
		case DOWN:
			
			break;
		}
	}
};

int main() {
	// initialize members
	Game game(2, 2);

	game.render();
	game.render();
	game.render();

	while (true) {
		//int key = _getch();
		//cout << key;
		
	}
}