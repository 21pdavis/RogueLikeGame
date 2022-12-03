#pragma once

#include "SDL.h"

namespace Colors {
	constexpr SDL_Color red {255, 0, 0, 255};
	constexpr SDL_Color green{ 0, 255, 0, 255 };
	constexpr SDL_Color blue{ 0, 0, 255, 255 };
	constexpr SDL_Color white{ 255, 255, 255, 255 };
	constexpr SDL_Color black{ 0, 0, 0, 255 };
}
