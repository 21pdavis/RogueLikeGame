#include "LevelMap.hpp"

#pragma optimize("", off)
// Pixel Class
// Public Interface
Pixel::Pixel(int x, int y) 
	: x(x), y(y), maxHeight(Height::GROUND) {}

Pixel::Pixel(int x, int y, std::unique_ptr<GameElement> startingElem)
	: x(x), y(y), maxHeight(Height::GROUND)
{
	stack.emplace_back(std::move(startingElem));
}

void Pixel::update()
{
	for (auto& elem : stack)
	{
		elem->update();
	}
}

void Pixel::render()
{
	if (stack.size() > 0)
	{
		stack.back()->render();
	}
}

void Pixel::addElem(std::unique_ptr<GameElement> elem)
{
	insertByHeight(std::move(elem));
}

std::unique_ptr<GameElement> Pixel::removeBySymbol(const char symbol)
{
	std::unique_ptr<GameElement> ret;
	for (auto& elem : stack)
	{
		if (elem->getSymbol() == symbol)
		{
			ret = std::move(stack.back());
			stack.pop_back();
			return std::move(ret);
			//return std::move(stack.back());
		}
	}
	return nullptr;
}

int Pixel::getX() const
{
	return x;
}

int Pixel::getY() const
{
	return y;
}

// Private Interface
void Pixel::insertByHeight(std::unique_ptr<GameElement> elemPtr)
{
	// update elemPtr height
	elemPtr->setXY(x, y);

	if (elemPtr->getHeight() >= maxHeight)
	{
		stack.push_back(std::move(elemPtr));
		return;
	}

	auto it = stack.begin();
	for (; it != stack.end(); ++it)
	{
		if (elemPtr->getHeight() > (*it)->getHeight())
		{
			stack.insert(it + 1, std::move(elemPtr));
			return;
		}
	}

	maxHeight = elemPtr->getHeight();
	// it == stack.end() --> insert is equivalent to push_back()
	stack.insert(it, std::move(elemPtr));
}

LevelMap::LevelMap()
	: playableX(0), playableY(0)
{
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			map[i][j] = std::make_unique<Pixel>(j, i);
		}
	}

	// add a tile on every pixel
	for (const auto& row : map)
	{
		for (const auto& pixel : row)
		{
			pixel->addElem(std::make_unique<Tile>(pixel->getX(), pixel->getY()));
		}
	}

	// add player at (0, 0)
	map[playableX][playableY]->addElem(std::make_unique<Player>(playableX, playableY));
}

void LevelMap::update()
{
	for (const auto& row : map)
	{
		for (const auto& pixel : row)
		{
			pixel->update();
		}
	}
}

/*
	Call the render() method of the element at the top of each Pixel's stack
*/
void LevelMap::render()
{
	for (auto& row : map)
	{
		for (auto& pixel : row)
		{
			pixel->render();
		}
	}
}

bool LevelMap::moveBySymbol(int x, int y, char symbol, std::pair<int, int> direction)
{
	if (x + direction.first >= 0 && x + direction.first < MAP_WIDTH &&
		y + direction.second >= 0 && y + direction.second < MAP_HEIGHT)
	{
		std::unique_ptr<GameElement> elem = map[y][x]->removeBySymbol(symbol);
		if (elem != nullptr) {
			map[y + direction.second][x + direction.first]->addElem(std::move(elem));
			return true;
		}
	}
	return false;
}
#pragma optimize("", on)

bool LevelMap::movePlayable(const std::pair<int, int>& direction)
{
	bool moved = moveBySymbol(playableX, playableY, Symbols::PLAYER, direction);

	if (moved)
	{
		playableX += direction.first;
		playableY += direction.second;
	}

	return moved;
}
