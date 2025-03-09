#pragma once

class LayoutManager;
class Renderer;
class Widget;
struct UIState;

class DebugRenderer {
public:
  DebugRenderer() = default;
  ~DebugRenderer() = default;

  void renderDebug(UIState& uiState, LayoutManager& layout, Renderer& renderer);

private:
  void drawWidgetDebugInfo(Widget& widget, UIState& uiState, Renderer& renderer);
};