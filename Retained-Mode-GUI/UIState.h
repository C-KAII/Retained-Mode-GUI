// UIState.h
#pragma once
#include "LayoutManager.h"
#include <vector>
#include <memory>
#include <algorithm>

struct UIState {
  UIState() = default;
  ~UIState() = default;

  bool handleEvents(LayoutManager& layout, Renderer& renderer) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {

      case SDL_MOUSEMOTION:
        mouseX = event.motion.x - scrollX;
        mouseY = event.motion.y - scrollY;
        if (debugMode && draggingWidget) {
          draggingWidget->setPosition(mouseX - dragOffset.x, mouseY - dragOffset.y);
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
          buttonX = event.button.x - scrollX;
          buttonY = event.button.y - scrollY;
          mouseDown = true;

          if (debugMode) {
            // In debug mode, try to pick up a widget
            for (auto& widget : layout.getWidgets()) {
              SDL_Rect rect = widget->getRect();
              if (
                buttonX >= rect.x && buttonX < rect.x + rect.w &&
                buttonY >= rect.y && buttonY < rect.y + rect.h
                ) {
                draggingWidget = widget.get();
                dragOffset.x = buttonX - rect.x;
                dragOffset.y = buttonY - rect.y;
                break;
              }
            }
          }
        }
        break;

      case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
          buttonX = event.button.x - scrollX;
          buttonY = event.button.y - scrollY;
          mouseDown = false;

          bool flag = false;
          for (const auto& widget : layout.getWidgets()) {
            if (regionHit(widget->getRect())) {
              flag = true;
              break;
            }
          }
          if (flag == false) {
            kbdItem = -1;
          }

          if (debugMode && draggingWidget) {
            int padding = 5;
            // On drop, see if we're over another widget to swap
            Widget* dropTarget = nullptr;
            bool insertBefore = false;

            for (auto& widget : layout.getWidgets()) {
              SDL_Rect rect = widget->getRect();
              // Ignore dragged widget
              if (widget.get() == draggingWidget) { continue; }
              if (
                buttonX >= rect.x - (padding / 2) && buttonX < rect.x + rect.w + (padding / 2) &&
                buttonY >= rect.y - (padding / 2) && buttonY < rect.y + rect.h + (padding / 2)
                ) {
                dropTarget = widget.get();
                // If mouse is on LHS, insert before
                insertBefore = (mouseX < (rect.x - (padding / 2)) + ((rect.w + (padding / 2)) / 2));
                break;
              }
            }

            // Widget insertion functionality
            if (dropTarget) {
              layout.reorderWidget(draggingWidget, dropTarget, insertBefore);
            }

            draggingWidget = nullptr;
            needsUpdate = true;
          }
        }
        break;

      case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0 && !(keyMod & KMOD_SHIFT)) {
          scrollY += 5; // Scroll up
        } else if (event.wheel.y < 0 && !(keyMod & KMOD_SHIFT)) {
          scrollY -= 5; // Scroll down
        }

        if (event.wheel.x > 0 || (keyMod & KMOD_SHIFT && event.wheel.y > 0)) {
          scrollX += 5; // Scroll left
        } else if (event.wheel.x < 0 || (keyMod & KMOD_SHIFT && event.wheel.y < 0)) {
          scrollX -= 5; // Scroll right
        }

        clampScrolling(layout, renderer);
        break;

      case SDL_KEYDOWN:
        keyEntered = event.key.keysym.sym;
        keyMod = event.key.keysym.mod;

        if (event.key.keysym.sym == SDLK_CAPSLOCK) {
          isCapsLockOn = !isCapsLockOn;
        }

        if (event.key.keysym.sym == SDLK_TAB && kbdItem == -1) {
          kbdItem = 0;
          keyEntered = 0;
        }
        break;

      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_F1:
          debugMode = !debugMode;
          break;
        case SDLK_F4:
          return false;
        default:
          break;
        }

        keyMod = 0;
        break;

      case SDL_TEXTINPUT:
        if (kbdItem != -1) {
          char c = event.text.text[0];

          if (std::isalpha(c)) {
            bool shiftActive = (keyMod & KMOD_SHIFT) != 0;
            if (shiftActive ^ isCapsLockOn) {
              c = std::toupper(c);
            } else {
              c = std::tolower(c);
            }
          } else {
            if ((keyMod & KMOD_SHIFT) != 0) {
              c = getShiftedChar(c);
            }
          }
          // Process key entered
          processKeyInput(c, layout);
        }
        break;

      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          int newWidth = event.window.data1;
          int newHeight = event.window.data2;
          renderer.updateWindowSize(newWidth, newHeight);
          needsUpdate = true;
        }
        break;

      case SDL_QUIT:
        return false;
      }
    }
    return true;
  }

  char getShiftedChar(char c) {
    switch (c) {
    case '`': return '~';
    case '1': return '!';
    case '2': return '@';
    case '3': return '#';
    case '4': return '$';
    case '5': return '%';
    case '6': return '^';
    case '7': return '&';
    case '8': return '*';
    case '9': return '(';
    case '0': return ')';
    case '-': return '_';
    case '=': return '+';
    case '[': return '{';
    case ']': return '}';
    case '\\': return '|';
    case ';': return ':';
    case '\'': return '"';
    case ',': return '<';
    case '.': return '>';
    case '/': return '?';
    default: return c;
    }
  }

  void processKeyInput(char c, LayoutManager& layout) {
    if (kbdItem != -1) {
      for (auto& widget : layout.getWidgets()) {
        if (widget->getId() == kbdItem && widget->getWidgetTypeEnum() == Widget::WidgetType::TEXT_FIELD) {
          widget->handleCharInput(c);
        }
      }
    }
  }

  bool regionHit(int x, int y, int w, int h) const {
    if (
      mouseX < x || mouseY < y ||
      mouseX >= x + w || mouseY >= y + h
      ) {
      return false;
    }
    return true;
  }

  bool regionHit(SDL_Rect rect) const {
    if (
      mouseX < rect.x || mouseY < rect.y ||
      mouseX >= rect.x + rect.w || mouseY >= rect.y + rect.h
      ) {
      return false;
    }
    return true;
  }

  void clampScrolling(const LayoutManager& layout, const Renderer& renderer) {
    scrollX = std::clamp(
      scrollX,
      std::min(0, -(layout.getGridWidth() - renderer.getScreenWidth()) - 210),
      0
    );

    scrollY = std::clamp(
      scrollY,
      std::min(0, -(layout.getGridHeight() - renderer.getScreenHeight()) - 210),
      0
    );
  }

  bool isClickBlocked() const { return regionHit(blockingArea); }

  void setBlockingArea(int x, int y, int w, int h) { blockingArea = { x, y, w, h }; }

  void setBlockingArea(SDL_Rect rect) { blockingArea = rect; }

  void resetBlockingArea() { blockingArea = { 0, 0, 0, 0 }; }

  int mouseX{ 0 };
  int mouseY{ 0 };
  int buttonX{ 0 };
  int buttonY{ 0 };
  int scrollX{ 0 };
  int scrollY{ 0 };
  bool mouseDown{ false };

  int hotItem{ 0 };
  int activeItem{ 0 };

  int kbdItem{ -1 };
  int lastWidget{ 0 };

  int keyEntered{ 0 };
  int keyMod{ 0 };
  bool isCapsLockOn{ false };

  bool needsUpdate{ true };
  bool debugMode{ false };

  SDL_Rect blockingArea{ 0, 0, 0, 0 };
  Widget* draggingWidget{ nullptr };
  SDL_Point dragOffset{ 0, 0 };
};