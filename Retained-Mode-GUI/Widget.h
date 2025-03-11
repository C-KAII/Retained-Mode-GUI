// Widget.h
#pragma once
#include "Renderer.h"
#include <SDL2/SDL.h>

constexpr int OUTLINE_PADDING = 2;

struct UIState;

class Widget {
public:
  enum class WidgetType {
    NONE,
    SPACER,
    TEXT_BOX,
    BUTTON,
    SLIDER,
    TEXT_FIELD,
    TOGGLE_SWITCH,
    DROPDOWN_MENU,
    SCROLLBAR
  };

  enum class LabelPosition {
    NONE,
    ABOVE,
    BELOW,
    INSIDE,
    LHS,
    RHS
  };

  Widget() = default;
  virtual ~Widget() = default;

  int getId() const { return m_id; }

  std::string getWidgetType() const {
    std::string result = "";
    switch (m_widgetType) {
    case WidgetType::NONE:
      result = "Error: Not a widget";
      break;
    case WidgetType::SPACER:
      result = "Spacer";
      break;
    case WidgetType::TEXT_BOX:
      result = "TextBox";
      break;
    case WidgetType::BUTTON:
      result = "Button";
      break;
    case WidgetType::SLIDER:
      result = "Slider";
      break;
    case WidgetType::TEXT_FIELD:
      result = "TextField";
      break;
    case WidgetType::TOGGLE_SWITCH:
      result = "ToggleSwitch";
      break;
    case WidgetType::DROPDOWN_MENU:
      result = "DropdownMenu";
      break;
    case WidgetType::SCROLLBAR:
      result = "Scrollbar";
      break;
    default:
      result = "Error: No matching widget types";
      break;
    }
    return result;
  }

  WidgetType getWidgetTypeEnum() const { return m_widgetType; }

  SDL_Rect getRect() { return { m_x, m_y, m_width, m_height }; }

  void setRect(int x, int y, int width, int height) {
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
  }

  void setRect(SDL_Rect rect) {
    m_x = rect.x;
    m_y = rect.y;
    m_width = rect.w;
    m_height = rect.h;
  }

  SDL_Color getColor() const { return m_color; }

  void setColor(SDL_Color color) { m_color = color; }

  LabelPosition getLabelPosition() const { return m_labelPos; }
  
  void setLabelPosition(LabelPosition pos) { m_labelPos = pos; }

  std::string getLabel() const { return m_label; }

  void setLabel(std::string label) { m_label = label; }

  SDL_Point getPosition() const { return { m_x, m_y }; }

  void setPosition(int x, int y) {
    m_x = x;
    m_y = y;
  }

  void setPosition(SDL_Point pos) {
    m_x = pos.x;
    m_y = pos.y;
  }

  void setSize(int width, int height) {
    m_width = width;
    m_height = height;
  }

  int getZIndex() const { return m_zIndex; }
  
  void setZIndex(int zIndex) { m_zIndex = zIndex; }

  SDL_Rect getPadding() const { return m_padding; }

  void setPadding(Renderer& renderer) {
    if (!m_label.empty()) {
      int padW = renderer.getFontWidth() * static_cast<int>(m_label.size()) + 4;
      int padH = renderer.getFontHeight() + 4;
      int extra = (padW > m_width) ? padW - m_width : 0;

      switch (m_labelPos) {
      case LabelPosition::ABOVE:
        m_padding = { 0, padH, extra, padH };
        break;
      case LabelPosition::BELOW:
        m_padding = { 0, 0, extra, padH };
        break;
      case LabelPosition::INSIDE:
        m_padding = { 0, 0, 0, 0 };
        m_width += extra;
        break;
      case LabelPosition::LHS:
        m_padding = { padW, 0, padW, 0 };
        break;
      case LabelPosition::RHS:
        m_padding = { 0, 0, padW, 0 };
        break;
      }
    }
  }

  void drawLabel(Renderer& renderer, int xPos, int yPos, LabelPosition customLabelPos = LabelPosition::NONE, std::string customLabel = "") const {
    LabelPosition labelPos = (customLabelPos == LabelPosition::NONE) ? m_labelPos : customLabelPos;
    std::string label = (customLabel.empty()) ? m_label : customLabel;
    
    switch (labelPos) {
    case LabelPosition::ABOVE:
      renderer.drawText(label, xPos, yPos - (renderer.getFontHeight() + 4));
      break;

    case LabelPosition::INSIDE:
      renderer.drawTextCentered(label, xPos, yPos, m_width, m_height, UTILS::getContrastingColor(m_color));
      break;

    case LabelPosition::BELOW:
      renderer.drawText(label, xPos, yPos + m_height + 4);
      break;

    case LabelPosition::LHS:
      renderer.drawText(label, xPos - m_padding.w, yPos);
      break;

    case LabelPosition::RHS:
      renderer.drawText(label, xPos + m_width + 4, yPos);
      break;
    case LabelPosition::NONE:
    default:
      break;
    }
  }

  virtual void update(Renderer& renderer, UIState& uiState) = 0;
  virtual void render(Renderer& renderer, const UIState& uiState) = 0;
  virtual void handleCharInput(char c) = 0;

protected:

  int m_id{ 0 };

  int m_x{ 0 };
  int m_y{ 0 };
  int m_width{ 0 };
  int m_height{ 0 };
  SDL_Rect m_padding{ 0, 0, 0, 0 };

  SDL_Color m_color{ UTILS::COLOR::WHITE };

  WidgetType m_widgetType{ WidgetType::NONE };
  std::string m_label;
  LabelPosition m_labelPos{ LabelPosition::ABOVE };

  int m_zIndex{ 0 };
};