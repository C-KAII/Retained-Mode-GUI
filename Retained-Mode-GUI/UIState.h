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

  void clampScrolling(const LayoutManager& layout, const Renderer& renderer) {
    // Set and clamp x-axis scrolling
    maxScrollX = std::min(0, -(layout.getGridWidth() - renderer.getScreenWidth()) - 210);
    scrollX = std::clamp(scrollX, maxScrollX, 0);
    // Set and clamp y-axis scrolling
    maxScrollY = std::min(0, -(layout.getGridHeight() - renderer.getScreenHeight()) - 210);
    scrollY = std::clamp(scrollY, maxScrollY, 0);
  }

  bool isClickBlocked() const {
    for (const auto& area : systemBlockingAreas) {
      if (regionHit(area)) { return true; }
    }
    // Finally check the widgetBlockingArea
    return regionHit(widgetBlockingArea);
  }

  void setWidgetBlockingArea(int x, int y, int w, int h) { widgetBlockingArea = { x, y, w, h }; }

  void setWidgetBlockingArea(SDL_Rect rect) { widgetBlockingArea = rect; }

  void resetWidgetBlockingArea() { widgetBlockingArea = { 0, 0, 0, 0 }; }

  void addSystemBlockingArea(const SDL_Rect& rect) { systemBlockingAreas.push_back(rect); }

  void clearSystemBlockingAreas() { systemBlockingAreas.clear(); }

  int mouseX{ 0 };
  int mouseY{ 0 };
  int leftMbX{ 0 };
  int leftMbY{ 0 };
  int rightMbX{ 0 };
  int rightMbY{ 0 };
  bool mouseDown{ false };

  int scrollX{ 0 };
  int scrollY{ 0 };
  int maxScrollX{ 0 };
  int maxScrollY{ 0 };

  int hotItem{ 0 };
  int activeItem{ 0 };

  int kbdItem{ -1 };
  int lastWidget{ 0 };

  int keyEntered{ 0 };
  int keyMod{ 0 };
  bool isCapsLockOn{ false };

  bool needsUpdate{ true };
  bool editMode{ false };
  bool debugMode{ false };

  bool isRightClickMenuOpen{ false };
  Widget* rightClickedWidget{ nullptr };

  Widget* draggingWidget{ nullptr };
  SDL_Point dragOffset{ 0, 0 };

  SDL_Rect widgetBlockingArea{ 0, 0, 0, 0 };
  std::vector<SDL_Rect> systemBlockingAreas;
};