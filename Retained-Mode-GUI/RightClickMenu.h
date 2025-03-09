#pragma once
#include "Renderer.h"
#include "UIState.h"
#include <vector>

class RightClickMenu {
public:
  std::string getOption(Renderer& renderer, UIState& uiState) {
    const int numOptions = (uiState.rightClickedWidget) ? 2 : 1;
    const SDL_Rect rect = {
      uiState.buttonRX + uiState.scrollX,
      uiState.buttonRY + uiState.scrollY,
      150, 30
    };

    uiState.setBlockingArea(rect.x, rect.y, rect.w, rect.h * numOptions);

    std::vector<std::string> options;
    if (uiState.rightClickedWidget) {
      options.push_back("Properties");
      options.push_back("Delete");
    } else {
      options.push_back("Add Widget");
    }

    int hotOptionY = -100;
    int selectedOption = -1;
    for (int i = 0; i < numOptions; ++i) {
      int optionY = rect.y + (i * rect.h);

      if (uiState.regionHit(rect.x - uiState.scrollX, optionY - uiState.scrollY, rect.w, rect.h)) {
        hotOptionY = optionY;
        if (uiState.mouseDown && selectedOption != i) {
          selectedOption = i;
          uiState.mouseDown = false;
          uiState.isRightClickMenuOpen = false;
          uiState.resetBlockingArea();
          return options[i];
        }
      }

      SDL_Color optionColor = (hotOptionY == optionY) ? UTILS::COLOR::WHITE : UTILS::COLOR::LIGHT_LIGHT_GREY;
      renderer.drawRect(rect.x, optionY, rect.w, rect.h, optionColor);
      renderer.drawTextCentered(options[i], rect.x, optionY, rect.w, rect.h);
    }

    // Deselect if clicked outside
    if (
      uiState.mouseDown &&
      !uiState.regionHit(rect.x - uiState.scrollX, rect.y - uiState.scrollY, rect.w, rect.h * numOptions)
      ) {
      uiState.mouseDown = false;
      uiState.isRightClickMenuOpen = false;
      uiState.resetBlockingArea();
    }
    return "";
  }
};