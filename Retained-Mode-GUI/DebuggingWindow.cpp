#include "DebuggingWindow.h"

bool DebuggingWindow::init(int width, int height) {
  m_width = width;
  m_height = height;

  m_window = SDL_CreateWindow("Debug Window",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    m_width, m_height, SDL_WINDOW_SHOWN);

  if (!m_window) {
    std::cerr << "Failed to create Debug Window: " << SDL_GetError() << std::endl;
    return false;
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if (!m_renderer) {
    std::cerr << "Failed to create Debug Renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  m_isOpen = true;
  return true;
}

void DebuggingWindow::update(UIState& uiState, LayoutManager& layout) {
  // Can track relevant info here like mouse, keyboard, layout area, etc. to display in debugging window
}

void DebuggingWindow::render() {
  if (!m_isOpen) { return; }

  SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
  SDL_RenderClear(m_renderer);

  // Render debug information (placeholder)
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_Rect debugBox = { 10, 10, 580, 380 };
  SDL_RenderDrawRect(m_renderer, &debugBox);

  SDL_RenderPresent(m_renderer);
}

void DebuggingWindow::cleanUp() {
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
  }

  if (m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }

  m_isOpen = false;
}