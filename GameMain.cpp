// Experiment.cpp : Main game class (TODO: Split up into multiple cpp files)

#include <iostream>
#include <array>
#include <vector>
#include "conio.h"
#include "GameHeader.h" // not used yet - may just delete in favor of a cpp file that contains the Element classes

// Constants //
// Keys
constexpr int LEFT = 75;
constexpr int UP = 72;
constexpr int RIGHT = 77;
constexpr int DOWN = 80;

//using std::vector;
using std::cout;
using std::endl;
using std::array;
using std::vector;

// static, nonmoving element for stationary objects (GameElements are static by default)
class GameElement {
protected:
	int x;
	int y;
	char symbol;
	bool passable;

	// constructor protected to prevent instantiation
	GameElement(int x, int y, char symbol, bool passable) : x(x), y(y), symbol(symbol), passable(passable) {}
};

// nonstatic, moving element for moving objects
class MovingElement : public GameElement {
protected:
	MovingElement(int x, int y, char symbol, bool passable) : GameElement(x, y, symbol, passable) {}

	virtual void move(int dx, int dy) {
		this->x += dx;
		this->y += dy;
	}
};

// TODO: Add an Entity/Creature class
// TODO: Add static vs. nonstatic GameElement classes

class Player : public MovingElement {
public:
	static const int MAX_HP = 100;
	static const int MIN_HP = 0;
	int health;

	Player(int x, int y, int health) : MovingElement(x, y, 'X', false), health(health) {}
	
	int getX() { return this->x; }

	int getY() { return this->y; }

	char getSymbol() { return this->symbol; }
};

class Tile : public GameElement {
public:
	Tile(int x, int y) : GameElement(x, y, '.', true) {}
};

class Grid {
private:
	static const int width = 5;
	static const int height = 5;

	array<array<GameElement&, width>, height> grid; 
	
	void overwrite(char replaceChar) {
		for (auto& row : grid) {
			for (int i = 0; i < this->width; i++) {
				cout << replaceChar;
			}
		}
	}

	void clear() {
		overwrite('\b');
	}
public:
	Grid() {
		// grid initially occupies unitialized memory, so have to manually sweep through it
		grid[0][0] = Player(0, 0, 100);
		for (int i = 0; i < this->height; i++) {
			for (int j = (i == 0 ? 1 : 0); j < this->width; j++) {
				grid[i][j] = Tile(i, j);
			}
		}
	}

	void update(char newChar, int x, int y) {
		this->grid[x][y] = newChar;
	}

	void draw(vector<GameElement>& elems) {
		this->clear();
		for (GameElement& elem : elems) {
			
		}
	}
};

class Game {
private:
	vector<GameElement> elements;
	Grid grid;
public:
	Game(vector<GameElement>& elements, Grid grid) : elements(elements), grid(grid) {}

	void moveElem(GameElement* elem, int dx, int dy) {
		this->player.move(dx, dy);
		this->grid.update(this->player.getSymbol(), this->player.getX(), this->player.getY());
	}
	
	void render() {
		grid.draw();
	}

	Game& getMutGame() { return *this; }

	const Game& getGame() const { return *this; }

	void HandleKey(int keynum, GameElement& elem) {
		// remember to check grid bounds
		switch (keynum) {
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
	while (true) {
		int key = _getch();
		cout << key;
	}
}