// Button.cpp
#include "Slider.h"

Slider::Slider(int id, int x, int y, int width, int height, int minValue, int maxValue, int& valueRef, std::string label, LabelPosition labelPos, SDL_Color color)
  : m_minValue(minValue), m_maxValue(maxValue), m_value(valueRef) {
  m_id = id;
  m_widgetType = WidgetType::SLIDER;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_label = label;
  m_labelPos = labelPos;
  m_color = color;
}

void Slider::update(Renderer& renderer, UIState& uiState) {
  // Check if button should be hot
  if (uiState.regionHit(m_x, m_y, m_width, m_height) && !uiState.isClickBlocked()) {
    uiState.hotItem = m_id;
    if (uiState.activeItem == 0 && uiState.mouseDown) {
      uiState.kbdItem = m_id;
      uiState.activeItem = m_id;
    }
  }

  // If no widget has keyboard focus, take it
  if (uiState.kbdItem == 0) {
    uiState.kbdItem = m_id;
  }

  // Keyboard interactions
  if (uiState.kbdItem == m_id) {
    switch (uiState.keyEntered) {
    case SDLK_TAB:
      uiState.kbdItem = 0;  // Lose focus
      if (uiState.keyMod & KMOD_SHIFT) {
        uiState.kbdItem = uiState.lastWidget;
      }
      uiState.keyEntered = 0;
      break;
    case SDLK_UP:
      if (m_value < m_maxValue) {
        m_value++;
      }
      break;
    case SDLK_DOWN:
      if (m_value > 0) {
        m_value--;
      }
      break;
    }
  }

  uiState.lastWidget = m_id;

  if (uiState.editMode) { return; }

  // Mouse drag logic
  if (uiState.activeItem == m_id) {
    int mousePos = uiState.mouseY - (m_y + 8);
    if (mousePos < 0) { mousePos = 0; }
    if (mousePos > m_height - m_width) { mousePos = m_height - m_width; }

    int newValue = m_maxValue - ((mousePos * (m_maxValue - m_minValue)) / (m_height - m_width));
    if (newValue != m_value) {
      m_value = newValue;
    }
  }

  if (m_value < m_minValue) { m_value = m_minValue; }
  if (m_value > m_maxValue) { m_value = m_maxValue; }
}

void Slider::render(Renderer& renderer, const UIState& uiState) {
  // If we have keyboard focus, show it
  if (uiState.kbdItem == m_id) {
    renderer.drawRect(
      m_x + uiState.scrollX - OUTLINE_PADDING, m_y + uiState.scrollY - OUTLINE_PADDING,
      m_width + (OUTLINE_PADDING * 2), m_height + (OUTLINE_PADDING * 2),
      UTILS::COLOR::RED
    );
  }

  // Draw slider track
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, m_color);

  // Handle color
  SDL_Color sliderColor = (uiState.activeItem == m_id || uiState.hotItem == m_id)
    ? UTILS::COLOR::WHITE : UTILS::COLOR::DARK_GREY;

  // Draw slider handle
  int yPos = (m_height - m_width) * (m_maxValue - m_value) / (m_maxValue - m_minValue);
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY + yPos, m_width, m_width, sliderColor);

  // Draw label
  if (!m_label.empty()) {
    drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY);
  }

  // Draw value
  drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY + yPos, LabelPosition::RHS, std::to_string(m_value));
}