#include "ScrollBar.h"

Scrollbar::Scrollbar(int id, int x, int y, int width, int height, int* scrollValue, ScrollType type)
  : m_scrollValue(scrollValue), m_scrollType(type) {
  m_id = id;
  m_widgetType = WidgetType::SCROLLBAR;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_color = UTILS::COLOR::LIGHT_GREY;
}

void Scrollbar::update(Renderer& renderer, UIState& uiState) {
  if (
    (uiState.maxScrollX == 0 && m_scrollType == ScrollType::HORIZONTAL) ||
    (uiState.maxScrollY == 0 && m_scrollType == ScrollType::VERTICAL)
    ) {
    return;
  }

  if (m_scrollType == ScrollType::HORIZONTAL) {
    m_width = renderer.getScreenWidth();
    m_y = renderer.getScreenHeight() - m_height;
  } else {
    m_height = renderer.getScreenHeight();
    m_x = renderer.getScreenWidth() - m_width;
  }

  uiState.addSystemBlockingArea({ m_x, m_y, m_width, m_height });

  SDL_Rect handleRect = getHandleRect(uiState);
  bool updateScroll = false;
  float scrollRatio = 0.0f;
  static int newHandlePos = (m_scrollType == ScrollType::HORIZONTAL) ? handleRect.x : handleRect.y;

  // Detect if clicking anywhere on scrollbar track
  if (uiState.regionHit(m_x - uiState.scrollX, m_y - uiState.scrollY, m_width, m_height)) {
    uiState.hotItem = m_id;

    if (uiState.mouseDown && uiState.activeItem == 0) {
      uiState.activeItem = m_id;

      if (uiState.regionHit(handleRect.x - uiState.scrollX, handleRect.y - uiState.scrollY, handleRect.w, handleRect.h)) {
        // Clicked on the handle, enable dragging
        m_dragging = true;
        m_dragOffset = (m_scrollType == ScrollType::HORIZONTAL)
          ? uiState.mouseX + uiState.scrollX - handleRect.x : uiState.mouseY + uiState.scrollY - handleRect.y;
      } else {
        // Clicked on the scrollbar track, move handle immediately
        if (m_scrollType == ScrollType::HORIZONTAL) {
          newHandlePos = std::clamp(uiState.mouseX + uiState.scrollX - (handleRect.w / 2), m_x, m_x + m_width - handleRect.w);
          scrollRatio = static_cast<float>(newHandlePos - m_x) / std::max(1, (m_width - handleRect.w));
        } else {
          newHandlePos = std::clamp(uiState.mouseY + uiState.scrollY - (handleRect.h / 2), m_y, m_y + m_height - handleRect.h);
          scrollRatio = static_cast<float>(newHandlePos - m_y) / std::max(1, (m_height - handleRect.h));
        }

        updateScroll = true;
      }
    }
  }

  if (m_dragging) {
    if (!uiState.mouseDown) {
      m_dragging = false;
    } else {
      if (m_prevMousePos != ((m_scrollType == ScrollType::HORIZONTAL) ? uiState.mouseX : uiState.mouseY)) {
        if (m_scrollType == ScrollType::HORIZONTAL) {
          newHandlePos = std::clamp(uiState.mouseX + uiState.scrollX - m_dragOffset, m_x, m_x + m_width - handleRect.w);
          scrollRatio = static_cast<float>(newHandlePos - m_x) / std::max(1, (m_width - handleRect.w));
        } else {
          newHandlePos = std::clamp(uiState.mouseY + uiState.scrollY - m_dragOffset, m_y, m_y + m_height - handleRect.h);
          scrollRatio = static_cast<float>(newHandlePos - m_y) / std::max(1, (m_height - handleRect.h));
        }

        updateScroll = true;
      }
    }
  }

  if (updateScroll) {
    *m_scrollValue = (m_scrollType == ScrollType::HORIZONTAL)
      ? static_cast<int>(scrollRatio * uiState.maxScrollX) : static_cast<int>(scrollRatio * uiState.maxScrollY);
  }

  m_prevMousePos = (m_scrollType == ScrollType::HORIZONTAL) ? uiState.mouseX : uiState.mouseY;
}

void Scrollbar::render(Renderer& renderer, const UIState& uiState) {
  if (
    (uiState.maxScrollX == 0 && m_scrollType == ScrollType::HORIZONTAL) ||
    (uiState.maxScrollY == 0 && m_scrollType == ScrollType::VERTICAL)
    ) {
    return;
  }

  renderer.drawRect(m_x, m_y, m_width, m_height, m_color);

  SDL_Rect handleRect = getHandleRect(uiState);
  SDL_Color handleColor = (uiState.activeItem == m_id) ? UTILS::COLOR::WHITE : UTILS::COLOR::DIM_GREY;
  renderer.drawRect(handleRect, handleColor);
}

int Scrollbar::getHandleWidth(const int maxScrollX) const {
  if (m_scrollType == ScrollType::HORIZONTAL) {
    return std::max(20, static_cast<int>(m_width * (1.0f - (static_cast<float>(std::abs(maxScrollX)) / std::max(1, m_width)))));
  } else {
    return m_width;
  }
}

int Scrollbar::getHandleHeight(const int maxScrollY) const {
  if (m_scrollType == ScrollType::HORIZONTAL) {
    return m_height;
  } else {
    return std::max(20, static_cast<int>(m_height * (1.0f - (static_cast<float>(std::abs(maxScrollY)) / std::max(1, m_height)))));
  }
}

SDL_Rect Scrollbar::getHandleRect(const UIState& uiState) const {
  int handleWidth = getHandleWidth(uiState.maxScrollX);
  int handleHeight = getHandleHeight(uiState.maxScrollY);

  if (m_scrollType == ScrollType::HORIZONTAL) {
    return {
      m_x + static_cast<int>((static_cast<float>(uiState.scrollX) / std::min(uiState.maxScrollX, -1)) * (m_width - handleWidth)), m_y,
      handleWidth, handleHeight
    };
  } else {
    return {
      m_x, m_y + static_cast<int>((static_cast<float>(uiState.scrollY) / std::min(uiState.maxScrollY, -1)) * (m_height - handleHeight)),
      handleWidth, handleHeight
    };
  }
}