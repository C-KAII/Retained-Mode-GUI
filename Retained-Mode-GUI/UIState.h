// UIState.h
#pragma once
#include "LayoutManager.h"
#include <vector>
#include <memory>
#include <algorithm>

struct UIState {
  UIState() = default;
  ~UIState() = default;

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

  bool isClickBlocked() const { return regionHit(blockingArea); }

  void setBlockingArea(int x, int y, int w, int h) { blockingArea = { x, y, w, h }; }

  void setBlockingArea(SDL_Rect rect) { blockingArea = rect; }

  void resetBlockingArea() { blockingArea = { 0, 0, 0, 0 }; }

  int mouseX{ 0 };
  int mouseY{ 0 };
  int buttonLX{ 0 };
  int buttonLY{ 0 };
  int buttonRX{ 0 };
  int buttonRY{ 0 };
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

  bool isRightClickMenuOpen{ false };
  Widget* rightClickedWidget{ nullptr };

  SDL_Rect blockingArea{ 0, 0, 0, 0 };
  Widget* draggingWidget{ nullptr };
  SDL_Point dragOffset{ 0, 0 };
};