#pragma once
#include "Widget.h"
#include "Renderer.h"
#include "UIState.h"
#include <vector>
#include <string>
#include <functional>

class DropdownMenu : public Widget {
public:
  DropdownMenu(
    int id, int x, int y, int width, int height,
    const std::vector<std::string>& options,
    std::function<void(const std::string&)> onSelect,
    std::string label = "", LabelPosition labelPos = LabelPosition::ABOVE, SDL_Color color = SDL_Color{ 170, 170, 170, 255 }
  );

  std::string getSelectedOption() const { return m_options[m_selectedIndex]; }

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

private:
  std::vector<std::string> m_options;
  int m_selectedIndex{ 0 };
  bool m_isExpanded{ false };
  std::function<void(const std::string&)> m_onSelect;
  int m_hotOptionY{ 0 };

};

