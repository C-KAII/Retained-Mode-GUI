#include "DebuggingWindow.h"

bool DebuggingWindow::init(int width, int height) {
  if (!m_renderer.init("Debug Window", width, height)) {
    return false;
  }
  if (!m_renderer.initFont("assets/fonts/OpenDyslexicMono-Regular.otf", 12)) {
    return false;
  }

  m_isOpen = true;
  return true;
}

void DebuggingWindow::render(const UIState& uiState, const LayoutManager& layout, const Renderer& renderer) {
  if (!m_isOpen) { return; }

  m_renderer.clear({ 30, 30, 30, 255 });

  drawDebugInfo(uiState, layout, renderer);

  m_renderer.present();
}

void DebuggingWindow::handleEvents(const SDL_Event& event) {
  if (!m_isOpen) { return; }

  switch (event.type) {
  case SDL_MOUSEWHEEL:
    if (event.wheel.y > 0) {
      m_scrollY += 10; // Scroll up
    } else if (event.wheel.y < 0) {
      m_scrollY -= 10; // Sroll down
    }

    m_scrollY = std::clamp(m_scrollY, m_maxScrollY, -10);
    break;

  case SDL_WINDOWEVENT:
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      int newWidth = event.window.data1;
      int newHeight = event.window.data2;
      m_renderer.updateWindowSize(newWidth, newHeight);
    }
    break;
  }
}

void DebuggingWindow::drawDebugInfo(const UIState& uiState, const LayoutManager& layout, const Renderer& renderer) {
  int xOffset = 10, yOffset = 10 + m_scrollY;
  SDL_Color textColor = UTILS::COLOR::WHITE;

  // Display UIState information
  m_renderer.drawText(
    "UIState:",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "Mouse: (" + std::to_string(uiState.mouseX) + ", " + std::to_string(uiState.mouseY) + ") -- " +
    "Mouse + Scroll: (" + std::to_string(uiState.mouseX + uiState.scrollX) + ", " + std::to_string(uiState.mouseY + uiState.scrollY) + ")",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "LMB: (" + std::to_string(uiState.leftMbX) + ", " + std::to_string(uiState.leftMbY) + ") -- " +
    "RMB: (" + std::to_string(uiState.rightMbX) + ", " + std::to_string(uiState.rightMbY) + ")",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "Scroll: (" + std::to_string(uiState.scrollX) + ", " + std::to_string(uiState.scrollY) + ") -- " +
    "MaxScroll: (" + std::to_string(uiState.maxScrollX) + ", " + std::to_string(uiState.maxScrollY) + ")",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "hotItem: " + std::to_string(uiState.hotItem) + " -- activeItem: " + std::to_string(uiState.activeItem) + " -- " +
    "kbdItem: " + std::to_string(uiState.kbdItem) + " -- lastWidget: " + std::to_string(uiState.lastWidget),
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "keyEntered: " + std::to_string(uiState.keyEntered) + " -- keyMod: " + std::to_string(uiState.keyMod),
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "isCapsLockOn: " + std::to_string(uiState.isCapsLockOn) + " -- isRightClickMenuOpen: " + std::to_string(uiState.isRightClickMenuOpen),
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "needsUpdate: " + std::to_string(uiState.needsUpdate) + " -- editMode: " + std::to_string(uiState.editMode),
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "draggingWidget ID: " + std::to_string((uiState.draggingWidget) ? uiState.draggingWidget->getId() : 0) + " -- "
    "dragOffset: (" + std::to_string(uiState.dragOffset.x) + ", " + std::to_string(uiState.dragOffset.y) + ")",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText(
    "widgetBlockingArea: (" + std::to_string(uiState.widgetBlockingArea.x) + ", " + std::to_string(uiState.widgetBlockingArea.y) + ", " +
    std::to_string(uiState.widgetBlockingArea.w) + ", " + std::to_string(uiState.widgetBlockingArea.h) + ")",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  for (auto& area : uiState.systemBlockingAreas) {
    m_renderer.drawText(
      "systemBlockingArea: (" + std::to_string(area.x) + ", " + std::to_string(area.y) + ", " +
      std::to_string(area.w) + ", " + std::to_string(area.h) + ")",
      xOffset, yOffset, textColor
    );
    yOffset += 20;
  }


  // Display LayoutManager information
  yOffset += 20;
  m_renderer.drawText(
    "LayoutManager:",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText("LayoutType: " + layout.getLayoutTypeText() + " -- needsUpdate: " + std::to_string(layout.needsUpdate()), xOffset, yOffset, textColor);
  yOffset += 20;
  m_renderer.drawText("Grid - W: " + std::to_string(layout.getGridWidth()) + " H: " + std::to_string(layout.getGridHeight()), xOffset, yOffset, textColor);
  yOffset += 20;
  m_renderer.drawText("Num Widgets: " + std::to_string(layout.getNumWidgets()) + " -- Num Columns: " + std::to_string(layout.getNumColumns()), xOffset, yOffset, textColor);
  yOffset += 20;

  int numRows = layout.getNumWidgets() / layout.getNumColumns();
  for (int i = 0; i < numRows; i++) {
    m_renderer.drawText("Row: " + std::to_string(i) + " -- Height: " + std::to_string(layout.getRowHeight(i)), xOffset, yOffset, textColor);
    xOffset += 25 * m_renderer.getFontWidth();
    if (i != numRows - 1 && xOffset + (25 * m_renderer.getFontWidth()) > m_renderer.getScreenWidth()) {
      xOffset = 10;
      yOffset += 20;
    }
  }
  xOffset = 10;
  yOffset += 20;


  // Display Renderer information
  yOffset += 20;
  m_renderer.drawText(
    "Renderer:",
    xOffset, yOffset, textColor
  );
  yOffset += 20;

  m_renderer.drawText("Main window ID: " + std::to_string(renderer.getWindowID()) + " -- Debug Window ID: " + std::to_string(m_renderer.getWindowID()), xOffset, yOffset, textColor);
  yOffset += 20;
  m_renderer.drawText(
    "Main Window - W: " + std::to_string(renderer.getScreenWidth()) + " H: " + std::to_string(renderer.getScreenHeight()) + " -- " +
    "Debug Window - W: " + std::to_string(m_renderer.getScreenWidth()) + " H: " + std::to_string(m_renderer.getScreenHeight()),
    xOffset, yOffset, textColor
  );
  yOffset += 20;
  m_renderer.drawText("Font - W: " + std::to_string(renderer.getFontWidth()) + " H: " + std::to_string(renderer.getFontHeight()), xOffset, yOffset, textColor);
  yOffset += 20;

  
  // Update maxScrollY if content is taller than window height
  m_maxScrollY = std::min(-10, -(yOffset - m_scrollY - m_renderer.getScreenHeight()) - 10);
}
