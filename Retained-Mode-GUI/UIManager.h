#pragma once
#include "UIState.h"
#include "LayoutManager.h"
#include "Renderer.h"

class UIManager {
public:
  bool handleEvents(UIState& uiState, LayoutManager& layout, Renderer& renderer) const;
  void processKeyInput(char c, int kbdItem, LayoutManager& layout) const;

  void addSystemWidget(std::unique_ptr<Widget> widget);
  void updateSystemWidgets(Renderer& renderer, UIState& uiState);
  void renderSystemWidgets(Renderer& renderer, const UIState& uiState);

private:
  std::vector<std::unique_ptr<Widget>> m_systemWidgets;
};