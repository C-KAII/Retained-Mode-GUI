// TextField.cpp
#include "TextField.h"

TextField::TextField(int id, int x, int y, int width, int height, std::string label, LabelPosition labelPos, SDL_Color color) {
  m_id = id;
  m_widgetType = WidgetType::TEXT_FIELD;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_color = color;
  m_label = label;
  m_labelPos = labelPos;
  m_lines.push_back("");
}

void TextField::update(Renderer& renderer, UIState& uiState) {
  // Check for hotness
  if (uiState.regionHit(m_x, m_y, m_width, m_height) && !uiState.isClickBlocked()) {
    uiState.hotItem = m_id;
    if (uiState.activeItem == 0 && uiState.mouseDown) {
      uiState.activeItem = m_id;
      uiState.kbdItem = m_id;
      m_isActive = true;
    }
  }

  // If no widget has keyboard focus, take it
  if (uiState.kbdItem == 0) {
    uiState.kbdItem = m_id;
  }

  // Deselect if clicked outside
  if (uiState.mouseDown && uiState.activeItem != m_id) {
    m_isActive = false;
  }

  // Handle keyboard input if focused
  if (uiState.kbdItem == m_id) {

    if (uiState.keyEntered == SDLK_TAB) {
      uiState.kbdItem = 0;  // Lose focus
      m_isActive = false;
      if (uiState.keyMod & KMOD_SHIFT) {
        uiState.kbdItem = uiState.lastWidget;
      }
      uiState.keyEntered = 0;
    }
    else if (!uiState.editMode) {
      switch (uiState.keyEntered) {

      case SDLK_BACKSPACE:
        if (m_cursorX > 0) {
          // Remove char from current line
          m_cursorX--;
          m_lines[m_cursorY].erase(m_lines[m_cursorY].begin() + m_cursorX);
        } else if (m_cursorY > 0) {
          // Merge with previous line if at start of current line
          m_cursorX = static_cast<int>(m_lines[m_cursorY - 1].size());
          m_lines[m_cursorY - 1] += m_lines[m_cursorY];
          m_lines.erase(m_lines.begin() + m_cursorY);
          m_cursorY--;
          uiState.needsUpdate = true;
        }
        break;

      case SDLK_RETURN:
        // Split current line at cursor position
        m_lines.insert(m_lines.begin() + m_cursorY + 1, m_lines[m_cursorY].substr(m_cursorX));
        m_lines[m_cursorY] = m_lines[m_cursorY].substr(0, m_cursorX);
        m_cursorY++;
        m_cursorX = 0;
        uiState.needsUpdate = true;
        break;

      case SDLK_LEFT:
        if (m_cursorX > 0) {
          m_cursorX--;
        } else if (m_cursorY > 0) {
          m_cursorY--;
          m_cursorX = static_cast<int>(m_lines[m_cursorY].size());
        }
        break;

      case SDLK_RIGHT:
        if (m_cursorX < static_cast<int>(m_lines[m_cursorY].size())) {
          m_cursorX++;
        } else if (m_cursorY < m_lines.size() - 1) {
          m_cursorY++;
          m_cursorX = 0;
        }
        break;
      }
    }
  }

  uiState.lastWidget = m_id;

  if (uiState.editMode) { return; }

  // Process text input
  if (uiState.kbdItem == m_id && m_inputChar != -1) {
    if (static_cast<int>(m_lines[m_cursorY].size()) < ((m_width - 4) / renderer.getFontWidth())) {
      m_lines[m_cursorY].insert(m_cursorX, 1, m_inputChar);
      m_cursorX++;
    } else {
      // If current line reaches max width, create a new line
      m_lines.insert(m_lines.begin() + m_cursorY + 1, std::string(1, m_inputChar));
      m_cursorY++;
      m_cursorX = 1;
      uiState.needsUpdate = true;
    }
    m_inputChar = -1;
  }

  m_height = static_cast<int>(m_lines.size()) * (renderer.getFontHeight() + 4);
}

void TextField::render(Renderer& renderer, const UIState& uiState) {
  // Draw outline if focused
  if (uiState.kbdItem == m_id) {
    renderer.drawRect(
      m_x + uiState.scrollX - OUTLINE_PADDING, m_y + uiState.scrollY - OUTLINE_PADDING,
      m_width + (OUTLINE_PADDING * 2), m_height + (OUTLINE_PADDING * 2),
      {255, 0, 0, 255}
    );
  }

  // Draw field background
  SDL_Color bgColor = (uiState.activeItem == m_id || uiState.hotItem == m_id) ? UTILS::COLOR::WHITE : m_color;
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, bgColor);

  // Render multi-line text
  SDL_Color textColor = UTILS::COLOR::BLACK;
  int lineSpacing = renderer.getFontHeight() + 4;
  for (int i = 0; i < static_cast<int>(m_lines.size()); i++) {
    renderer.drawText(m_lines[i], m_x + uiState.scrollX + 4, m_y + uiState.scrollY + 4 + i * lineSpacing, textColor);
  }
  if (m_lines[0].empty() && m_lines.size() <= 1) {
    std::string fieldText = (m_labelPos == LabelPosition::INSIDE) ? m_label : "Type here...";
    renderer.drawText(fieldText, m_x + uiState.scrollX + 4, m_y + uiState.scrollY + 4, textColor);
  }

  // Render blinking cursor
  if (uiState.kbdItem == m_id && (SDL_GetTicks() >> 8) & 1) {
    int cursorOffsetX = m_cursorX * renderer.getFontWidth();
    int cursorOffsetY = m_cursorY * lineSpacing;
    renderer.drawText("_", m_x + uiState.scrollX + 4 + cursorOffsetX, m_y + uiState.scrollY + 4 + cursorOffsetY, textColor);
  }

  // Draw label
  if (!m_label.empty() && m_labelPos != LabelPosition::INSIDE) {
    drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY);
  }
}