#pragma once
#include "Renderer.h"
#include "UIState.h"
#include "LayoutManager.h"
#include <SDL2/SDL.h>

class DebuggingWindow {
public:
  DebuggingWindow() = default;
  ~DebuggingWindow() { cleanUp(); }

  bool init(int width = 640, int height = 480);
  void render(const UIState& uiState, const LayoutManager& layout, const Renderer& renderer);
  void handleEvents(const SDL_Event& event);
  void cleanUp() {
    m_renderer.cleanUp();
    m_isOpen = false;
  }

  bool isOpen() const { return m_isOpen; }

  Uint32 getWindowID() const { return m_renderer.getWindowID(); }

private:
  void drawDebugInfo(const UIState& uiState, const LayoutManager& layout, const Renderer& renderer);

  Renderer m_renderer;
  bool m_isOpen{ false };

  int m_scrollY{ 0 };
  int m_maxScrollY{ 0 };
};