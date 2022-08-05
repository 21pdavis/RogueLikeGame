#pragma once

#include <array>
#include <vector>
#include <span>
#include <stack>
#include <string>
#include <memory>
#include <utility>
#include <variant>
//#include <optional>
#include <algorithm>
#include <curses.h>

//Constants 
//Keys
enum class Keys {
	KEY_ESC = 27
};

enum class Height : int {
	GROUND = 0,
	ON_GROUND,
	IN_AIR
};

enum class Symbols : char {
	Player = 'P',
	Editor = 'E'
};

namespace SymbolSets {
	const char Playable[2] = {'P', 'E'};
}

// static, nonmoving element for stationary objects
class GameElement {
protected:
	int y;
	int x;
	char symbol;
	bool noclip;
	Height height;

	GameElement(const GameElement& other) = delete;

	// constructor protected to prevent instantiation
	GameElement(int y, int x, char symbol, bool noclip, Height height)
		: y(y), x(x), symbol(symbol), noclip(noclip), height(height) {}
public:
	virtual int getY() const {
		return y;
	}

	virtual void setY(int newY) {
		y = newY;
	}

	virtual int getX() const {
		return x;
	}

	virtual void setX(int newX) {
		x = newX;
	}

	virtual char getSymbol() const {
		return symbol;
	}

	virtual const bool isNoClip() const {
		return noclip;
	}

	virtual void setPosition(int newY, int newX) {
		y = newY;
		x = newX;
	}

	virtual int getHeight() const {
		return static_cast<int>(height);
	}

	virtual bool operator==(const GameElement& other) const {
		return symbol == other.getSymbol();
	}
};

// structures and unmoving elements
class Tile : public GameElement {
public:
	Tile(int y, int x)
		: GameElement(y, x, '.', true, Height::GROUND) {}
};

class Placeable : public GameElement {
protected:
	Placeable(int y, int x, char symbol, bool noclip, Height height)
		: GameElement(y, x, symbol, noclip, height) {}
};

class HorizontalWall : public Placeable {
public:
	HorizontalWall(int y, int x)
		: Placeable(y, x, '_', false, Height::ON_GROUND) {}
};

class VerticalWall : public Placeable {
public:
	VerticalWall(int y, int x)
		: Placeable(y, x, '|', false, Height::ON_GROUND) {}
};

// nonstatic, moving element for moving objects
class MovingElement : public GameElement {
protected:
	MovingElement(int y, int x, char symbol, bool noclip, Height height)
		: GameElement(y, x, symbol, noclip, height) {}

public:
	virtual void move(int dy, int dx) {
		y += dy;
		x += dx;
	}
};


// moving creature elements
class Creature : public MovingElement {
protected:
	static const int MAX_HP = 100;
	static const int MIN_HP = 0;
	int health;

	Creature(int y, int x, char symbol, bool noclip, Height height, int health)
		: MovingElement(y, x, symbol, noclip, height), health(health) {}
};

class Human : public Creature {
protected:
	Human(int y, int x, char symbol, bool noclip, Height height, int health)
		: Creature(x, y, symbol, noclip, height, health) {}
};

class Playable : public Creature {
public:
	static constexpr int d_Y = 0;
	static constexpr int d_X = 0;


	virtual bool isEditor() = 0;

protected:
	Playable(char symbol, bool noclip, Height height, int health)
		: Creature(d_Y, d_X, symbol, noclip, height, health) {}
};

class Editor : public Playable {
public:
	Editor()
		: Playable('E', true, Height::IN_AIR, 100) {}

	bool isEditor() {
		return true;
	}
};

class Player : public Playable {
public:
	Player(int health)
		: Playable('P', false, Height::ON_GROUND, health) {}

	bool isEditor() {
		return false;
	}
};
