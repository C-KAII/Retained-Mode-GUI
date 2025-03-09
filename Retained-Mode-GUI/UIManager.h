#pragma once
#include "UIState.h"
#include "LayoutManager.h"
#include "Renderer.h"

class UIManager {
public:
  bool handleEvents(UIState& uiState, LayoutManager& layout, Renderer& renderer);
  //void handleRightClickSelection(UIState& uiState, LayoutManager& layout);
  void processKeyInput(char c, int kbdItem, LayoutManager& layout) const;
  void clampScrolling(UIState& uiState, const int gridWidth, const int gridHeight, const int screenWidth, const int screenHeight);
};