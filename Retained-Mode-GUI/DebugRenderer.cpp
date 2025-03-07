#include "DebugRenderer.h"
#include <iostream>

void DebugRenderer::renderDebug() {
  auto& widgets = m_layout.getWidgets();

  // Draw outlines and IDs for each widget
  for (auto& widget : widgets) {
    widget->update(m_renderer, m_uiState);

    SDL_Rect rect = widget->getRect();
    if (
      rect.x + rect.w + m_uiState.scrollX > 0 &&
      rect.y + rect.h + m_uiState.scrollY > 0 &&
      rect.x + m_uiState.scrollX < m_renderer.getScreenWidth() &&
      rect.y + m_uiState.scrollY < m_renderer.getScreenHeight()
      ) {
      m_renderer.drawRect(
        rect.x + m_uiState.scrollX, rect.y + m_uiState.scrollY,
        rect.w, rect.h, UTILS::COLOR::WHITE
      );

      m_renderer.drawText(
        std::to_string(widget->getId()),
        rect.x + m_uiState.scrollX + 2,
        rect.y + m_uiState.scrollY + 2,
        UTILS::COLOR::BLACK
      );
    }
  }

  // If there's a hot widget, show debug info
  if (m_uiState.hotItem != 0) {
    for (auto& widget : widgets) {
      if (m_uiState.hotItem == widget->getId()) {
        drawWidgetDebugInfo(*widget);
        break;
      }
    }
  }

  // Display Mouse Position
  m_renderer.drawText(
    '(' + std::to_string(m_uiState.mouseX) + ", " + std::to_string(m_uiState.mouseY) + ')',
    m_uiState.mouseX + m_uiState.scrollX,
    m_uiState.mouseY + m_uiState.scrollY - m_renderer.getFontHeight(),
    UTILS::COLOR::BLACK
  );


}

// private

void DebugRenderer::drawWidgetDebugInfo(Widget& widget) {
  SDL_Rect rect = widget.getRect();
  const int debugBoxX = rect.x + m_uiState.scrollX + rect.w + 10;
  const int debugBoxY = rect.y + m_uiState.scrollY;

  m_renderer.drawRect(debugBoxX - 2, debugBoxY - 2, 200 + 4, 200 + 4, UTILS::COLOR::RED);
  m_renderer.drawRect(debugBoxX, debugBoxY, 200, 200, UTILS::COLOR::WHITE);

  const int linePadding = m_renderer.getFontHeight() + 2;
  int yPos = debugBoxY + 2;

  // Render Widget Type
  m_renderer.drawText(
    "Type: " + widget.getWidgetType(),
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Real Position
  m_renderer.drawText(
    "real(" + std::to_string(rect.x) + ',' + std::to_string(rect.y) + ',' +
    std::to_string(rect.w) + ',' + std::to_string(rect.h) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Screen Position
  m_renderer.drawText(
    "screen(" + std::to_string(rect.x + m_uiState.scrollX) + ',' +
    std::to_string(rect.y + m_uiState.scrollY) + ',' +
    std::to_string(rect.w) + ',' + std::to_string(rect.h) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Color Preview
  SDL_Color color = widget.getColor();
  m_renderer.drawRect(debugBoxX + 2, yPos, m_renderer.getFontWidth() * 4, m_renderer.getFontHeight(), color);
  m_renderer.drawText(
    "rgba(" + std::to_string(color.r) + ',' + std::to_string(color.g) + ',' +
    std::to_string(color.b) + ',' + std::to_string(color.a) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );
}