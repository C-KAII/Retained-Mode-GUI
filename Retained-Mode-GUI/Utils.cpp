#include "Utils.h"

namespace UTILS {

SDL_Color getContrastingColor(const SDL_Color& color) {
  // Calculate luminance
  double luminance = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);

  // Return black if luminance is high, otherwise white
  return (luminance > 128) ? COLOR::BLACK : COLOR::WHITE;
}

char getShiftedChar(char c) {
  switch (c) {
  case '`': return '~';
  case '1': return '!';
  case '2': return '@';
  case '3': return '#';
  case '4': return '$';
  case '5': return '%';
  case '6': return '^';
  case '7': return '&';
  case '8': return '*';
  case '9': return '(';
  case '0': return ')';
  case '-': return '_';
  case '=': return '+';
  case '[': return '{';
  case ']': return '}';
  case '\\': return '|';
  case ';': return ':';
  case '\'': return '"';
  case ',': return '<';
  case '.': return '>';
  case '/': return '?';
  default: return c;
  }
}

}