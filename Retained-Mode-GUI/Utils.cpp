#include "Utils.h"

namespace UTILS {

SDL_Color getContrastingColor(const SDL_Color& color) {
  // Calculate luminance
  double luminance = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);

  // Return black if luminance is high, otherwise white
  return (luminance > 128) ? COLOR::BLACK : COLOR::WHITE;
}

}