#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

class Spacer : public Widget {
public:
  Spacer(int id, int x, int y, int width, int height) {
    m_id = id;
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_widgetType = WidgetType::SPACER;
  }

  ~Spacer() override = default;

  void update(Renderer& renderer, UIState& uiState) override {
    if (uiState.regionHit(m_x, m_y, m_width, m_height)) {
      uiState.hotItem = m_id;
    }
  }

  void render(Renderer& renderer, const UIState& uiState) override {}
  void handleCharInput(char c) override {}
};