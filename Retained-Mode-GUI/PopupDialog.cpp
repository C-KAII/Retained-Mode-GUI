#include "PopupDialog.h"

PopupDialog::PopupDialog(
  int id, int x, int y, int width, int height,
  const std::string& title, DialogType type,
  SDL_Color color
) : m_title(title), m_type(type) {
  m_id = id;
  m_widgetType = WidgetType::POPUP_DIALOG;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_color = color;
}

void PopupDialog::addTextField(const std::string& label, std::string& linkedValue) {
  int textFieldY = m_y + 40 + static_cast<int>(m_textFields.size()) * 30;
  m_textFields.push_back(std::make_unique<TextField>(
    m_id + static_cast<int>(m_textFields.size()), // TODO - Incorrect method of generating uids
    m_x + 10, textFieldY,
    m_width - 20, 0,
    label
    //linkedValue // TODO - wheres linkedValue being used
  ));
  // TODO - i dont think above is correct
}

void PopupDialog::addButton(const std::string& label, std::function<void()> onClick) {
  int buttonY = m_y + m_height - 40;
  int buttonX = m_x + 10 + static_cast<int>(m_buttons.size()) * 110;
  m_buttons.push_back(std::make_unique<Button>(
    m_id + static_cast<int>(m_buttons.size()), // TODO - Incorrect method of generating uids
    buttonX, buttonY,
    100, 30,
    onClick, label, LabelPosition::INSIDE, SDL_Color{200, 200, 200, 255}
  ));
}

void PopupDialog::update(Renderer& renderer, UIState& uiState) {
  if (!m_visible) { return; }

  for (auto& field : m_textFields) {
    field->update(renderer, uiState);
  }

  for (auto& button : m_buttons) {
    button->update(renderer, uiState);
  }
}

void PopupDialog::render(Renderer& renderer, const UIState& uiState) {
  if (!m_visible) { return; }

  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, UTILS::COLOR::WHITE);
  renderer.drawText(m_title, m_x + 10 + uiState.scrollX, m_y + 10 + uiState.scrollY, UTILS::COLOR::BLACK);

  int messageY = m_y + 30 + uiState.scrollY;
  for (const auto& msg : m_messages) {
    renderer.drawText(msg, m_x + 10 + uiState.scrollX, messageY, UTILS::COLOR::BLACK);
    messageY += 20;
  }

  for (auto& field : m_textFields) {
    field->render(renderer, uiState);
  }

  for (auto& button : m_buttons) {
    button->render(renderer, uiState);
  }
}