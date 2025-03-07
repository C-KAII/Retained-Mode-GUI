#pragma once
#include "Widget.h"
#include "Renderer.h"
#include "UIState.h"
#include <functional>

class ToggleSwitch : public Widget {
public:
  ToggleSwitch(
    int id, int x, int y, int width, int height,
    bool* linkedState = nullptr, std::function<void(bool)> onToggle = nullptr,
    std::string label = "", LabelPosition labelPos = LabelPosition::ABOVE, SDL_Color color = SDL_Color{ 0, 200, 0, 255 }
  );

  bool getState() const { return m_linkedState ? *m_linkedState : false; }

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

private:
  bool m_state{ false };  // Default OFF if no linked state
  bool* m_linkedState{ nullptr };  // Pointer to external state (can be null)
  std::function<void(bool)> m_onToggle;
  bool m_clicked{ false };
};