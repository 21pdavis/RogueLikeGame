#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <memory>
#include <curses.h>

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
