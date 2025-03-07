#include "TextBox.h"

TextBox::TextBox(
  int id, int x, int y, int width, int height,
  int fontWidth, int fontHeight,
  std::string text, SDL_Color textColor,
  std::string label, LabelPosition labelPos, SDL_Color color
) : m_textColor(textColor) {
  m_id = id;
  m_widgetType = WidgetType::TEXT_BOX;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_label = label;
  m_labelPos = labelPos;
  m_color = color;

  m_lines.push_back("");

  if (text.empty()) { return; }
  
  int cursorX = 0;
  int cursorY = 0;

  for (int i = 0; i < static_cast<int>(text.size()); i++) {
    // Process text to fit within box
    if (static_cast<int>(m_lines[cursorY].size()) < ((m_width - 4) / fontWidth)) {
      m_lines[cursorY].insert(cursorX, 1, text[i]);
      cursorX++;
    } else {
      // If current line reaches max width, create a new line
      m_lines.insert(m_lines.begin() + cursorY + 1, std::string(1, text[i]));
      cursorY++;
      cursorX = 1;
    }
  }

  m_height = static_cast<int>(m_lines.size()) * (fontHeight + 4);
}

void TextBox::update(Renderer& renderer, UIState& uiState) {
  if (uiState.regionHit(m_x, m_y, m_width, m_height)) {
    uiState.hotItem = m_id;
  }
}

void TextBox::render(Renderer& renderer, const UIState& uiState) {
  // Draw field background
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, m_color);

  // Render multi-line text
  int lineSpacing = renderer.getFontHeight() + 4;
  for (int i = 0; i < static_cast<int>(m_lines.size()); i++) {
    renderer.drawText(m_lines[i], m_x + uiState.scrollX + 4, m_y + uiState.scrollY + 4 + i * lineSpacing, m_textColor);
  }

  // Draw label
  if (!m_label.empty() && m_labelPos != LabelPosition::INSIDE) {
    drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY);
  }
}