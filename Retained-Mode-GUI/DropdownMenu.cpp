#include "DropdownMenu.h"

DropdownMenu::DropdownMenu(
  int id, int x, int y, int width, int height,
  const std::vector<std::string>& options,
  std::function<void(const std::string&)> onSelect,
  std::string label, LabelPosition labelPos, SDL_Color color
) : m_options(options), m_onSelect(onSelect) {
  m_id = id;
  m_widgetType = WidgetType::DROPDOWN_MENU;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_label = label;
  m_labelPos = labelPos;
  m_color = color;
}

void DropdownMenu::update(Renderer& renderer, UIState& uiState) {
  // Check if dropdown is clicked
  if (uiState.regionHit(m_x, m_y, m_width, m_height) && !uiState.isClickBlocked()) {
    uiState.hotItem = m_id;

    if (uiState.mouseDown && uiState.activeItem == 0) {
      uiState.kbdItem = m_id;
      uiState.activeItem = m_id;
      m_isExpanded = !m_isExpanded;  // Toggle dropdown

      if (m_isExpanded) {
        m_hotOptionY = (m_hotOptionY > m_y && m_hotOptionY <= m_y + (m_height * static_cast<int>(m_options.size())))
          ? m_y + m_height + (m_height * m_selectedIndex) : m_y;
      }
    }
  }

  if (m_isExpanded) {
    SDL_Rect dropdownArea = { m_x, m_y, m_width, m_height * (static_cast<int>(m_options.size() + 1)) };
    uiState.setWidgetBlockingArea(dropdownArea);
  }

  if (uiState.kbdItem == 0) {
    uiState.kbdItem = m_id;
  }

  if (uiState.kbdItem == m_id) {
    switch (uiState.keyEntered) {
    case SDLK_TAB:
      uiState.kbdItem = 0;
      m_isExpanded = false;
      if (uiState.keyMod & KMOD_SHIFT) {
        uiState.kbdItem = uiState.lastWidget;
      }
      uiState.keyEntered = 0;
      break;
    case SDLK_RETURN:
      uiState.hotItem = m_id;
      uiState.activeItem = m_id;

      if (m_isExpanded) {
        int selectedIndex = (m_hotOptionY - m_y - m_height) / m_height;
        if (selectedIndex >= 0 && selectedIndex < static_cast<int>(m_options.size())) {
          m_selectedIndex = selectedIndex;
          m_onSelect(m_options[m_selectedIndex]);
        }
      }

      if (m_onSelect) {
        m_isExpanded = !m_isExpanded;
        if (m_isExpanded) {
          m_hotOptionY = (m_hotOptionY > m_y && m_hotOptionY <= m_y + (m_height * static_cast<int>(m_options.size())))
            ? m_y + m_height + (m_height * m_selectedIndex) : m_y;
        }
      }
      break;
    case SDLK_UP:
      if (m_isExpanded) {
        m_hotOptionY -= m_height;

        if (m_hotOptionY < m_y + m_height) {
          m_hotOptionY = m_y + (m_height * static_cast<int>(m_options.size()));
        }
      }
      break;
    case SDLK_DOWN:
      if (m_isExpanded) {
        m_hotOptionY += m_height;

        if (m_hotOptionY > m_y + (m_height * static_cast<int>(m_options.size()))) {
          m_hotOptionY = m_y + m_height;
        }
      }
      break;
    }
  }

  uiState.lastWidget = m_id;

  if (uiState.debugMode) {
    m_isExpanded = false;
    setZIndex(0);
    return;
  }

  // Handle option selection when expanded
  if (m_isExpanded) {
    for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
      int optionY = m_y + m_height + (i * m_height);

      if (uiState.regionHit(m_x, optionY, m_width, m_height)) {
        uiState.hotItem = m_id;
        m_hotOptionY = optionY;

        if (uiState.mouseDown && m_selectedIndex != i) {
          m_selectedIndex = i;
          if (m_onSelect) {
            m_onSelect(m_options[m_selectedIndex]);  // Trigger callback
          }
        }
      }
    }
  }

  // Deselect if clicked outside
  if (m_isExpanded && uiState.mouseDown && uiState.activeItem != m_id || uiState.keyEntered == SDLK_ESCAPE) {
    m_isExpanded = false;
    uiState.mouseDown = false; // prevent clicking through dropdown
    uiState.resetWidgetBlockingArea();
  }

  setZIndex(m_isExpanded ? 100 : 0);
}

void DropdownMenu::render(Renderer& renderer, const UIState& uiState) {
  if (uiState.kbdItem == m_id) {
    renderer.drawRect(
      m_x + uiState.scrollX - OUTLINE_PADDING, m_y + uiState.scrollY - OUTLINE_PADDING,
      m_width + (OUTLINE_PADDING * 2), m_height + (OUTLINE_PADDING * 2),
      UTILS::COLOR::RED
    );
  }

  // Render main button background
  SDL_Color bgColor = (uiState.hotItem == m_id || uiState.activeItem == m_id) ? UTILS::COLOR::WHITE : m_color;
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, bgColor);

  // Render selected option text
  SDL_Color textColor = UTILS::COLOR::BLACK;
  renderer.drawText(m_options[m_selectedIndex], m_x + 5 + uiState.scrollX, m_y + 5 + uiState.scrollY, textColor);

  // Render dropdown arrow indicator
  renderer.drawText("v", m_x + m_width - 15 + uiState.scrollX, m_y + 5 + uiState.scrollY, textColor);

  if (m_isExpanded) {
    // Render dropdown list if expanded
    for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
      int optionY = m_y + m_height + (i * m_height);

      SDL_Color optionColor = (m_hotOptionY == optionY) ? UTILS::COLOR::WHITE : UTILS::COLOR::LIGHT_LIGHT_GREY;

      renderer.drawRect(m_x + uiState.scrollX, optionY + uiState.scrollY, m_width, m_height, optionColor);
      renderer.drawText(m_options[i], m_x + 5 + uiState.scrollX, optionY + 5 + uiState.scrollY, UTILS::COLOR::BLACK);
    }
  }

  // Draw label
  if (!m_label.empty() && m_labelPos != LabelPosition::INSIDE) {
    drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY);
  }
}