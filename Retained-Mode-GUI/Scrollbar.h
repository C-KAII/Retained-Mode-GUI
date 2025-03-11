#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"

class Scrollbar : public Widget {
public:
  enum class ScrollType {
    VERTICAL,
    HORIZONTAL
  };

  Scrollbar(int id, int x, int y, int width, int height, int* scrollValue, ScrollType type);
  ~Scrollbar() = default;

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  virtual void handleCharInput(char c) {}

private:
  int getHandleWidth(const int maxScrollX) const;
  int getHandleHeight(const int maxScrollY) const;
  SDL_Rect getHandleRect(const UIState& uiState) const;

  ScrollType m_scrollType;
  int* m_scrollValue;
  bool m_dragging{ false };
  int m_dragOffset{ 0 };
  int m_prevMousePos{ 0 };
};