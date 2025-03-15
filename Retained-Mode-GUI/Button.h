// Button.h
#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <functional>

class Button : public Widget {
public:
  Button(
    int id, int x, int y, int width, int height,
    std::function<void()> onClick = nullptr,
    std::string label = "", LabelPosition labelPos = LabelPosition::INSIDE,
    SDL_Color color = UTILS::COLOR::VERY_DARK_GREY
  );

  ~Button() override = default;

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

private:
  std::function<void()> m_onClick;
  bool m_clicked{ false };
};