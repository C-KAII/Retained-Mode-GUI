#pragma once
#include <SDL2/SDL.h>

namespace UTILS {


///////////////
// Constants //
///////////////

// Colour constants
namespace COLOR {

constexpr SDL_Color BLACK = { 0, 0, 0, 255 };
constexpr SDL_Color WHITE = { 255, 255, 255, 255 };
constexpr SDL_Color VERY_DARK_GREY = { 50, 50, 50, 255 };
constexpr SDL_Color DIM_GREY = { 105, 105, 105, 255 };
constexpr SDL_Color GREY = { 119, 119, 119, 255 };
constexpr SDL_Color DARK_GREY = { 169, 169, 169, 255 };
constexpr SDL_Color LIGHT_GREY = { 211, 211, 211, 255 };
constexpr SDL_Color LIGHT_LIGHT_GREY = { 230, 230, 230, 255 };
constexpr SDL_Color RED = { 255, 0, 0, 255 };
constexpr SDL_Color GREEN = { 0, 255, 0, 255 };
constexpr SDL_Color BLUE = { 0, 0, 255, 255 };

}



///////////
// Types //
///////////




///////////////
// Functions //
///////////////

SDL_Color getContrastingColor(const SDL_Color& color);

char getShiftedChar(char c);


}