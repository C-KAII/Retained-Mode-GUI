#include "UIManager.h"
#include "IDGenerator.h"
#include "Button.h"

bool UIManager::handleEvents(UIState& uiState, LayoutManager& layout, Renderer& renderer, DebuggingWindow& debugger) const {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Process events for any open windows

    switch (event.type) {
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_F1:
        uiState.editMode = !uiState.editMode;
        break;
      case SDLK_F2:
        uiState.debugMode = !uiState.debugMode;
        break;
      case SDLK_F4:
        return false;
      }
      break;

    case SDL_WINDOWEVENT:
      // Check if debugging window should close
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        uiState.debugMode = false;
      }
      break;
    }

    // Don't process remaining events if not current window
    if (event.window.windowID != renderer.getWindowID()) {
      debugger.handleEvents(event);
      continue;
    }

    switch (event.type) {
    case SDL_MOUSEMOTION:
      uiState.mouseX = event.motion.x - uiState.scrollX;
      uiState.mouseY = event.motion.y - uiState.scrollY;
      if (uiState.editMode && uiState.draggingWidget) {
        uiState.draggingWidget->setPosition(uiState.mouseX - uiState.dragOffset.x, uiState.mouseY - uiState.dragOffset.y);
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        uiState.mouseDown = true;
        uiState.leftMbX = event.button.x - uiState.scrollX;
        uiState.leftMbY = event.button.y - uiState.scrollY;

        if (uiState.editMode) {
          // In debug mode, try to pick up a widget
          for (auto& widget : layout.getWidgets()) {
            SDL_Rect rect = widget->getRect();
            if (
              uiState.leftMbX >= rect.x && uiState.leftMbX < rect.x + rect.w &&
              uiState.leftMbY >= rect.y && uiState.leftMbY < rect.y + rect.h
              ) {
              uiState.draggingWidget = widget.get();
              uiState.dragOffset.x = uiState.leftMbX - rect.x;
              uiState.dragOffset.y = uiState.leftMbY - rect.y;
              break;
            }
          }
        }
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        uiState.rightMbX = event.button.x - uiState.scrollX;
        uiState.rightMbY = event.button.y - uiState.scrollY;
        uiState.rightClickedWidget = nullptr;

        // Check if right click on widget
        for (auto& widget : layout.getWidgets()) {
          SDL_Rect rect = widget->getRect();
          if (
            uiState.rightMbX >= rect.x && uiState.rightMbX < rect.x + rect.w &&
            uiState.rightMbY >= rect.y && uiState.rightMbY < rect.y + rect.h
            ) {
            uiState.rightClickedWidget = widget.get();
            break;
          }
        }

        uiState.isRightClickMenuOpen = true;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      if (event.button.button == SDL_BUTTON_LEFT) {
        uiState.mouseDown = false;
        uiState.leftMbX = event.button.x - uiState.scrollX;
        uiState.leftMbY = event.button.y - uiState.scrollY;

        bool flag = false;
        for (const auto& widget : layout.getWidgets()) {
          if (uiState.regionHit(widget->getRect())) {
            flag = true;
            break;
          }
        }
        if (flag == false) {
          uiState.kbdItem = -1;
        }

        if (uiState.editMode && uiState.draggingWidget) {
          int padding = 5;
          // On drop, see if we're over another widget to swap
          Widget* dropTarget = nullptr;
          bool insertBefore = false;

          for (auto& widget : layout.getWidgets()) {
            SDL_Rect rect = widget->getRect();
            // Ignore dragged widget
            if (widget.get() == uiState.draggingWidget) { continue; }
            if (
              uiState.leftMbX >= rect.x - (padding / 2) && uiState.leftMbX < rect.x + rect.w + (padding / 2) &&
              uiState.leftMbY >= rect.y - (padding / 2) && uiState.leftMbY < rect.y + rect.h + (padding / 2)
              ) {
              dropTarget = widget.get();
              // If mouse is on LHS, insert before
              insertBefore = (uiState.mouseX < (rect.x - (padding / 2)) + ((rect.w + (padding / 2)) / 2));
              break;
            }
          }

          // Widget insertion functionality
          if (dropTarget) {
            layout.reorderWidget(uiState.draggingWidget, dropTarget, insertBefore);
          }

          uiState.draggingWidget = nullptr;
          uiState.needsUpdate = true;
        }
      }
      break;

    case SDL_MOUSEWHEEL:
      if (event.wheel.y > 0 && !(uiState.keyMod & KMOD_SHIFT)) {
        uiState.scrollY += 10; // Scroll up
      } else if (event.wheel.y < 0 && !(uiState.keyMod & KMOD_SHIFT)) {
        uiState.scrollY -= 10; // Scroll down
      }

      if (event.wheel.x > 0 || (uiState.keyMod & KMOD_SHIFT && event.wheel.y > 0)) {
        uiState.scrollX += 10; // Scroll left
      } else if (event.wheel.x < 0 || (uiState.keyMod & KMOD_SHIFT && event.wheel.y < 0)) {
        uiState.scrollX -= 10; // Scroll right
      }

      uiState.clampScrolling(layout, renderer);
      break;

    case SDL_KEYDOWN:
      uiState.keyEntered = event.key.keysym.sym;
      uiState.keyMod = event.key.keysym.mod;

      if (event.key.keysym.sym == SDLK_CAPSLOCK) {
        uiState.isCapsLockOn = !uiState.isCapsLockOn;
      }

      if (event.key.keysym.sym == SDLK_TAB && uiState.kbdItem == -1) {
        uiState.kbdItem = 0;
        uiState.keyEntered = 0;
      }
      break;

    case SDL_KEYUP:
      uiState.keyMod = 0;
      break;

    case SDL_TEXTINPUT:
      if (uiState.kbdItem != -1) {
        char c = event.text.text[0];

        if (std::isalpha(c)) {
          bool shiftActive = (uiState.keyMod & KMOD_SHIFT) != 0;
          if (shiftActive ^ uiState.isCapsLockOn) {
            c = std::toupper(c);
          } else {
            c = std::tolower(c);
          }
        } else {
          if ((uiState.keyMod & KMOD_SHIFT) != 0) {
            c = UTILS::getShiftedChar(c);
          }
        }
        // Process key entered
        processKeyInput(c, uiState.kbdItem, layout);
      }
      break;

    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        int newWidth = event.window.data1;
        int newHeight = event.window.data2;
        renderer.updateWindowSize(newWidth, newHeight);
        uiState.needsUpdate = true;

      } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) { return false; }
      break;

    case SDL_QUIT:
      return false;
    }
  }
  return true;
}

void UIManager::processKeyInput(char c, int kbdItem, LayoutManager& layout) const {
  if (kbdItem != -1) {
    for (auto& widget : layout.getWidgets()) {
      if (widget->getId() == kbdItem && widget->getWidgetTypeEnum() == Widget::WidgetType::TEXT_FIELD) {
        widget->handleCharInput(c);
      }
    }
  }
}

void UIManager::addSystemWidget(std::unique_ptr<Widget> widget) {
  m_systemWidgets.push_back(std::move(widget));
}

void UIManager::updateSystemWidgets(Renderer& renderer, UIState& uiState) {
  for (auto& widget : m_systemWidgets) {
    widget->update(renderer, uiState);
  }
}

void UIManager::renderSystemWidgets(Renderer& renderer, const UIState& uiState) {
  for (auto& widget : m_systemWidgets) {
    widget->render(renderer, uiState);
  }
}