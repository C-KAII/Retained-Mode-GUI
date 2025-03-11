#pragma once
#include "Renderer.h"
#include "UIState.h"
#include "LayoutManager.h"
#include <SDL2/SDL.h>

class DebuggingWindow {
public:
  DebuggingWindow() : m_width(640), m_height(480) {}
  ~DebuggingWindow() { cleanUp(); }

  bool init(int width = 640, int height = 480);
  void update(UIState& uiState, LayoutManager& layout);
  void render();
  //void handleEvents(bool& debugMode);
  void cleanUp();

  bool isOpen() const { return m_isOpen; }

  Uint32 getWindowID() const { return SDL_GetWindowID(m_window); }

private:
  SDL_Window* m_window{ nullptr };
  SDL_Renderer* m_renderer{ nullptr };
  bool m_isOpen{ false };

  int m_width;
  int m_height;
};