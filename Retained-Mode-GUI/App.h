// App.h
#pragma once
#include "Renderer.h"
#include "LayoutManager.h"
#include "UIState.h"
#include "UIManager.h"
#include "DebugRenderer.h"
#include "RightClickMenu.h"
#include "DebuggingWindow.h"
#include "Graph.h"
#include <SDL2/SDL_ttf.h>

class App {
public:
  App() = default;

  ~App() {
    SDL_StopTextInput();
    m_renderer.closeFont();
    TTF_Quit();
    SDL_Quit();
  }

  bool init();
  void run();

private:
  void addWidgets();
  void addSystemWidgets();
  void updateFPS();
  void rmguiPrepare();
  void rmguiFinish();
  void render();
  void renderHelp();

  Renderer m_renderer;
  LayoutManager m_layout;
  UIState m_uiState;
  UIManager m_uiManager;
  DebugRenderer m_debugRenderer;
  RightClickMenu m_rightClickMenu;
  DebuggingWindow m_debuggingWindow;

  std::vector<std::pair<float, float>> m_fpsData;
  Uint32 m_frameTime{ 0 };

  int m_redValue{ UTILS::COLOR::GREY.r };
  int m_greenValue{ UTILS::COLOR::GREY.g };
  int m_blueValue{ UTILS::COLOR::GREY.b };

  bool m_running{ false };
};