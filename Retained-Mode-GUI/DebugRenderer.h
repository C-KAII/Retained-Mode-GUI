#pragma once
#include "Renderer.h"
#include "UIState.h"
#include "LayoutManager.h"

class DebugRenderer {
public:
  DebugRenderer(Renderer& renderer, UIState& uiState, LayoutManager& layout)
    : m_renderer(renderer), m_uiState(uiState), m_layout(layout) {}

  ~DebugRenderer() = default;

  void renderDebug();

private:
  void drawWidgetDebugInfo(Widget& widget);

  Renderer& m_renderer;
  UIState& m_uiState;
  LayoutManager& m_layout;
};