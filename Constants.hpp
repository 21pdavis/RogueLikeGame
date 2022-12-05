#pragma once

#include <utility>

namespace Directions
{
	inline std::pair<int, int> UP = { 0, -1 };
	inline std::pair<int, int> DOWN = { 0, 1 };
	inline std::pair<int, int> LEFT = { -1, 0 };
	inline std::pair<int, int> RIGHT = { 1, 0 };
}

namespace Symbols
{
	inline char PLAYER = 'P';
	inline char HWALL = '-';
	inline char VWALL = '|';
	inline char TILE = '.';
}
