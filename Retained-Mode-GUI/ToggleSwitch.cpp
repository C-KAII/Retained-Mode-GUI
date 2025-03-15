#include "ToggleSwitch.h"

ToggleSwitch::ToggleSwitch(
  int id, int x, int y, int width, int height,
  bool* linkedState, std::function<void(bool)> onToggle,
  std::string label, LabelPosition labelPos, SDL_Color color
) : m_linkedState(linkedState), m_onToggle(onToggle) {
  m_id = id;
  m_widgetType = WidgetType::TOGGLE_SWITCH;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_label = label;
  m_labelPos = labelPos;
  m_color = color;

  if (!m_linkedState) { m_state = false; }
}

void ToggleSwitch::update(Renderer& renderer, UIState& uiState) {
  bool& stateRef = m_linkedState ? *m_linkedState : m_state; // Use linked state if available

  if (uiState.regionHit(m_x, m_y, m_width, m_height) && !uiState.isClickBlocked()) {
    uiState.hotItem = m_id;
    if (uiState.activeItem == 0 && uiState.mouseDown) {
      uiState.kbdItem = m_id;
      uiState.activeItem = m_id;
    }
  }

  if (uiState.kbdItem == 0) {
    uiState.kbdItem = m_id;
  }

  if (uiState.kbdItem == m_id) {
    switch (uiState.keyEntered) {
    case SDLK_TAB:
      uiState.kbdItem = 0;
      if (uiState.keyMod & KMOD_SHIFT) {
        uiState.kbdItem = uiState.lastWidget;
      }
      uiState.keyEntered = 0;
      break;
    case SDLK_RETURN:
      uiState.hotItem = m_id;
      uiState.activeItem = m_id;
      if (m_onToggle) {
        m_clicked = true;
      }
      break;
    }
  }

  uiState.lastWidget = m_id;

  if (uiState.editMode) {
    m_clicked = false;
    return;
  }

  if (
    !uiState.mouseDown &&
    uiState.hotItem == m_id &&
    uiState.activeItem == m_id &&
    m_onToggle
    ) {
    m_clicked = true;
    stateRef = !stateRef;  // Toggle state
  }

  if (m_clicked) {
    m_onToggle(stateRef);
    m_clicked = false;
  }
}

void ToggleSwitch::render(Renderer& renderer, const UIState& uiState) {
  if (uiState.kbdItem == m_id) {
    renderer.drawRect(
      m_x + uiState.scrollX - OUTLINE_PADDING, m_y + uiState.scrollY - OUTLINE_PADDING,
      m_width + (OUTLINE_PADDING * 2), m_height + (OUTLINE_PADDING * 2),
      UTILS::COLOR::RED
    );
  }

  bool state = m_linkedState ? *m_linkedState : m_state; // Use linked state if available

  SDL_Color bgColor = state ? m_color : UTILS::COLOR::GREY;
  SDL_Color knobColor = (uiState.activeItem == m_id || uiState.hotItem == m_id)
    ? UTILS::COLOR::WHITE : UTILS::COLOR::DARK_GREY;

  // Background (toggle area)
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, bgColor);

  // Knob position (left for off, right for on)
  int knobX = state ? (m_x + m_width - m_height) : m_x;

  // Knob (circle)
  renderer.drawRect(knobX + uiState.scrollX, m_y + uiState.scrollY, m_height, m_height, knobColor);

  // Draw label
  if (!m_label.empty()) {
    drawLabel(renderer, m_x + uiState.scrollX, m_y + uiState.scrollY);
  }
}