// Slider.h
#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class TextField : public Widget {
public:
  TextField(int id, int x, int y, int width, int height, std::string label = "", LabelPosition labelPos = LabelPosition::INSIDE, SDL_Color color = SDL_Color{ 170, 170, 170, 255 });
  ~TextField() override = default;

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override { m_inputChar = c; }

private:
  bool m_isActive{ false };
  char m_inputChar{ -1 };
  std::vector<std::string> m_lines;
  int m_cursorX{ 0 }; // X position of cursor
  int m_cursorY{ 0 }; // Y position of cursor (line number)
};