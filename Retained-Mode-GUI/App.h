// App.h
#pragma once
#include "Renderer.h"
#include "LayoutManager.h"
#include "UIState.h"
#include "DebugRenderer.h"
#include "PopupDialog.h"
#include <SDL2/SDL.h>

class App {
public:
  App() = default;
  ~App() = default;

  bool init();
  void run();

private:
  void addWidgets();
  void imguiPrepare();
  void imguiFinish();
  void render();
  void renderHelp();

  Renderer m_renderer{ };
  LayoutManager m_layout;
  UIState m_uiState{ };
  DebugRenderer m_debugRenderer{ m_renderer, m_uiState, m_layout };

  std::unique_ptr<PopupDialog> m_addWidgetDialog;
  std::string widgetName;
  
  int m_redValue{ UTILS::COLOR::GREY.r };
  int m_greenValue{ UTILS::COLOR::GREY.g };
  int m_blueValue{ UTILS::COLOR::GREY.b };

  bool m_running{ false };
};