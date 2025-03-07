#pragma once
#include "Widget.h"
#include "Button.h"
#include "TextField.h"
#include <vector>
#include <functional>

class PopupDialog : public Widget {
public:
  enum class DialogType { MESSAGE, FORM };

  PopupDialog(
    int id, int x, int y, int width, int height,
    const std::string& title, DialogType type,
    SDL_Color color = UTILS::COLOR::DARK_GREY
  );

  void addMessage(const std::string& message) { m_messages.push_back(message); }
  void addTextField(const std::string& label, std::string& linkedValue);
  void addButton(const std::string& label, std::function<void()> onClick);

  void show() { m_visible = true; }
  void hide() { m_visible = false; }
  bool isVisible() const { return m_visible; }

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

private:
  std::string m_title;
  DialogType m_type;
  std::vector<std::string> m_messages;
  std::vector<std::unique_ptr<TextField>> m_textFields;
  std::vector<std::unique_ptr<Button>> m_buttons;

  bool m_visible{ false };
};

