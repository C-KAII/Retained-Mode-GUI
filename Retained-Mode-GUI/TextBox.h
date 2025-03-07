// Text.h
#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

class TextBox : public Widget {
public:
  TextBox(
    int id, int x, int y, int width, int height,
    int fontWidth, int fontHeight,
    std::string text, SDL_Color textColor = UTILS::COLOR::BLACK,
    std::string label = "", LabelPosition labelPos = LabelPosition::ABOVE, SDL_Color color = UTILS::COLOR::GREY);

  ~TextBox() override = default;

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

private:
  std::vector<std::string> m_lines;
  SDL_Color m_textColor;
};