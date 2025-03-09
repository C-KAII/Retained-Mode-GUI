#include "UIManager.h"
#include "IDGenerator.h"
#include "Button.h"

bool UIManager::handleEvents(UIState& uiState, LayoutManager& layout, Renderer& renderer) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {

    case SDL_MOUSEMOTION:
      uiState.mouseX = event.motion.x - uiState.scrollX;
      uiState.mouseY = event.motion.y - uiState.scrollY;
      if (uiState.debugMode && uiState.draggingWidget) {
        uiState.draggingWidget->setPosition(uiState.mouseX - uiState.dragOffset.x, uiState.mouseY - uiState.dragOffset.y);
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        uiState.buttonLX = event.button.x - uiState.scrollX;
        uiState.buttonLY = event.button.y - uiState.scrollY;
        uiState.mouseDown = true;

        if (uiState.debugMode) {
          // In debug mode, try to pick up a widget
          for (auto& widget : layout.getWidgets()) {
            SDL_Rect rect = widget->getRect();
            if (
              uiState.buttonLX >= rect.x && uiState.buttonLX < rect.x + rect.w &&
              uiState.buttonLY >= rect.y && uiState.buttonLY < rect.y + rect.h
              ) {
              uiState.draggingWidget = widget.get();
              uiState.dragOffset.x = uiState.buttonLX - rect.x;
              uiState.dragOffset.y = uiState.buttonLY - rect.y;
              break;
            }
          }
        }
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        uiState.buttonRX = event.button.x - uiState.scrollX;
        uiState.buttonRY = event.button.y - uiState.scrollY;
        uiState.rightClickedWidget = nullptr;

        // Check if right click on widget
        for (auto& widget : layout.getWidgets()) {
          SDL_Rect rect = widget->getRect();
          if (
            uiState.buttonRX >= rect.x && uiState.buttonRX < rect.x + rect.w &&
            uiState.buttonRY >= rect.y && uiState.buttonRY < rect.y + rect.h
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

        //if (uiState.isRightClickMenuOpen) {
        //  handleRightClickSelection(uiState, layout);
        //}
        //uiState.isRightClickMenuOpen = false;

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

        if (uiState.debugMode && uiState.draggingWidget) {
          int padding = 5;
          // On drop, see if we're over another widget to swap
          Widget* dropTarget = nullptr;
          bool insertBefore = false;

          for (auto& widget : layout.getWidgets()) {
            SDL_Rect rect = widget->getRect();
            // Ignore dragged widget
            if (widget.get() == uiState.draggingWidget) { continue; }
            if (
              uiState.buttonLX >= rect.x - (padding / 2) && uiState.buttonLX < rect.x + rect.w + (padding / 2) &&
              uiState.buttonLY >= rect.y - (padding / 2) && uiState.buttonLY < rect.y + rect.h + (padding / 2)
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

      clampScrolling(uiState, layout.getGridWidth(), layout.getGridHeight(), renderer.getScreenWidth(), renderer.getScreenHeight());
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
      switch (event.key.keysym.sym) {
      case SDLK_F1:
        uiState.debugMode = !uiState.debugMode;
        break;
      case SDLK_F4:
        return false;
      default:
        break;
      }

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
      }
      break;

    case SDL_QUIT:
      return false;
    }
  }
  return true;
}

//void UIManager::handleRightClickSelection(UIState& uiState, LayoutManager& layout) {
//  if (uiState.rightClickedWidget) {
//    // Handle widget-specific actions
//    if (uiState.buttonRY >= uiState.buttonRY + 20 && uiState.buttonRY < uiState.buttonRY + 40) {
//      std::cout << "Properties of Widget ID: " << uiState.rightClickedWidget->getId() << "\n";
//    }
//    if (uiState.buttonRY >= uiState.buttonRY + 40 && uiState.buttonRY < uiState.buttonRY + 60) {
//      std::cout << "Delete Widget ID: " << uiState.rightClickedWidget->getId() << "\n";
//      layout.removeWidget(uiState.rightClickedWidget);
//    }
//  } else {
//    // Handle empty-space actions
//    if (uiState.buttonRY >= uiState.buttonRY + 20 && uiState.buttonRY < uiState.buttonRY + 40) {
//      std::cout << "Adding new widget...\n";
//      layout.addWidget(std::make_unique<Button>(GEN_ID, uiState.buttonRX, uiState.buttonRY, 100, 50));
//    }
//  }
//}

void UIManager::processKeyInput(char c, int kbdItem, LayoutManager& layout) const {
  if (kbdItem != -1) {
    for (auto& widget : layout.getWidgets()) {
      if (widget->getId() == kbdItem && widget->getWidgetTypeEnum() == Widget::WidgetType::TEXT_FIELD) {
        widget->handleCharInput(c);
      }
    }
  }
}

void UIManager::clampScrolling(UIState& uiState, const int gridWidth, const int gridHeight, const int screenWidth, const int screenHeight) {
  uiState.scrollX = std::clamp(
    uiState.scrollX,
    std::min(0, -(gridWidth - screenWidth) - 210),
    0
  );

  uiState.scrollY = std::clamp(
    uiState.scrollY,
    std::min(0, -(gridHeight - screenHeight) - 210),
    0
  );
}