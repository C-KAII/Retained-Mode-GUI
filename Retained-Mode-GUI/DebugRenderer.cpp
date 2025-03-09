#include "DebugRenderer.h"
#include "UIState.h"
#include "Renderer.h"
#include "LayoutManager.h"
#include <iostream>

void DebugRenderer::renderDebug(UIState& uiState, LayoutManager& layout, Renderer& renderer) {
  auto& widgets = layout.getWidgets();

  // Draw outlines and IDs for each widget
  for (auto& widget : widgets) {
    widget->update(renderer, uiState);

    SDL_Rect rect = widget->getRect();
    if (
      rect.x + rect.w + uiState.scrollX > 0 &&
      rect.y + rect.h + uiState.scrollY > 0 &&
      rect.x + uiState.scrollX < renderer.getScreenWidth() &&
      rect.y + uiState.scrollY < renderer.getScreenHeight()
      ) {
      renderer.drawRect(
        rect.x + uiState.scrollX, rect.y + uiState.scrollY,
        rect.w, rect.h, UTILS::COLOR::WHITE
      );

      renderer.drawText(
        std::to_string(widget->getId()),
        rect.x + uiState.scrollX + 2,
        rect.y + uiState.scrollY + 2,
        UTILS::COLOR::BLACK
      );
    }
  }

  // If there's a hot widget, show debug info
  if (uiState.hotItem != 0) {
    for (auto& widget : widgets) {
      if (uiState.hotItem == widget->getId()) {
        drawWidgetDebugInfo(*widget, uiState, renderer);
        break;
      }
    }
  }

  // Display Mouse Position
  renderer.drawText(
    '(' + std::to_string(uiState.mouseX) + ", " + std::to_string(uiState.mouseY) + ')',
    uiState.mouseX + uiState.scrollX,
    uiState.mouseY + uiState.scrollY - renderer.getFontHeight(),
    UTILS::COLOR::BLACK
  );
}

// private

// TODO - move this to each widget so we can display more info
void DebugRenderer::drawWidgetDebugInfo(Widget& widget, UIState& uiState, Renderer& renderer) {
  SDL_Rect rect = widget.getRect();
  const int debugBoxX = rect.x + uiState.scrollX + rect.w + 10;
  const int debugBoxY = rect.y + uiState.scrollY;

  renderer.drawRect(debugBoxX - 2, debugBoxY - 2, 200 + 4, 200 + 4, UTILS::COLOR::RED);
  renderer.drawRect(debugBoxX, debugBoxY, 200, 200, UTILS::COLOR::WHITE);

  const int linePadding = renderer.getFontHeight() + 2;
  int yPos = debugBoxY + 2;

  // Render Widget Type
  renderer.drawText(
    "Type: " + widget.getWidgetType(),
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Real Position
  renderer.drawText(
    "real(" + std::to_string(rect.x) + ',' + std::to_string(rect.y) + ',' +
    std::to_string(rect.w) + ',' + std::to_string(rect.h) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Screen Position
  renderer.drawText(
    "screen(" + std::to_string(rect.x + uiState.scrollX) + ',' +
    std::to_string(rect.y + uiState.scrollY) + ',' +
    std::to_string(rect.w) + ',' + std::to_string(rect.h) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );

  yPos += linePadding;

  // Render Color Preview
  SDL_Color color = widget.getColor();
  renderer.drawRect(debugBoxX + 2, yPos, renderer.getFontWidth() * 4, renderer.getFontHeight(), color);
  renderer.drawText(
    "rgba(" + std::to_string(color.r) + ',' + std::to_string(color.g) + ',' +
    std::to_string(color.b) + ',' + std::to_string(color.a) + ')',
    debugBoxX + 2, yPos, UTILS::COLOR::BLACK
  );
}