// App.cpp
#include "App.h"
#include "IDGenerator.h"
#include "Spacer.h"
#include "TextBox.h"
#include "Button.h"
#include "Slider.h"
#include "TextField.h"
#include "ToggleSwitch.h"
#include "DropdownMenu.h"
#include <iostream>
#include <cstdlib> // rand()
#include <algorithm> // sort

bool App::init() {
  if (!m_renderer.init("RMGUI", 640, 480)) {
    return false;
  }
  if (!m_renderer.initFont("assets/fonts/OpenDyslexicMono-Regular.otf", 12)) {
    return false;
  }
  return true;
}

void App::run() {
  addWidgets();
  m_layout.applyLayout();

  m_running = true;
  while (m_running) {
    render();

    m_running &= m_uiManager.handleEvents(m_uiState, m_layout, m_renderer);

    if (m_layout.needsUpdate() || m_uiState.needsUpdate) {
      m_layout.applyLayout();
      m_uiState.needsUpdate = false;
    }
  }
}

// private

void App::addWidgets() {
  // Text box
  m_layout.addWidget(std::make_unique<TextBox>(
    GEN_ID, 0, 0, 200, 40,
    m_renderer.getFontWidth(), m_renderer.getFontHeight(),
    "This is a text box.",
    UTILS::COLOR::BLACK,
    "Test labelling"
  ));

  m_layout.addWidget(std::make_unique<TextBox>(
    GEN_ID, 0, 0, 210, 40,
    m_renderer.getFontWidth(), m_renderer.getFontHeight(),
    "Here lies another text box, but this one is much longer, and may need more space to render it's lines."
  ));

  // Toggle Switches
  m_layout.addWidget(std::make_unique<ToggleSwitch>(
    GEN_ID, 0, 0, 60, 30, nullptr,
    [](bool state) {
      std::cout << "Standalone state toggle: " << (state ? "ON" : "OFF") << std::endl;
    }
  ));

  m_layout.addWidget(std::make_unique<ToggleSwitch>(
    GEN_ID, 0, 0, 60, 30, &m_uiState.debugMode,
    [](bool state) {},
    "Debug Mode"
  ));

  // Dropdown Menus
  m_layout.addWidget(std::make_unique<DropdownMenu>(
    GEN_ID, 0, 0, 150, 30,
    std::vector<std::string>{"Option 1", "Option 2", "Option 3"},
    [](const std::string& selected) {
      std::cout << "Dropdown 1 selected option: " << selected << std::endl;
    },
    "Dropdown Menu"
  ));

  m_layout.addWidget(std::make_unique<DropdownMenu>(
    GEN_ID, 0, 0, 150, 30,
    std::vector<std::string>{"Option 1", "Option 2", "Option 3"},
    [](const std::string& selected) {
      std::cout << "Dropdown 2 selected option: " << selected << std::endl;
    }
  ));

  // Buttons
  m_layout.addWidget(std::make_unique<Button>(
    GEN_ID, 0, 0, 100, 50,
    [this]() {
      std::cout << "Blue button clicked!\n";
      int numCols = m_layout.getNumColumns() - 1;
      if (numCols >= 1) {
        m_layout.setNumColumns(numCols);
      }
    },
    "Columns--",
    Widget::LabelPosition::INSIDE,
    UTILS::COLOR::BLUE
  ));

  m_layout.addWidget(std::make_unique<Button>(
    GEN_ID, 0, 0, 100, 50,
    [this]() {
      std::cout << "Green button clicked!\n";
      int numCols = m_layout.getNumColumns() + 1;
      if (numCols <= m_layout.getNumWidgets()) {
        m_layout.setNumColumns(numCols);
      }
    },
    "Columns++",
    Widget::LabelPosition::INSIDE,
    UTILS::COLOR::GREEN
  ));

  m_layout.addWidget(std::make_unique<Button>(
    GEN_ID, 0, 0, 100, 50,
    [this]() {
      std::cout << "Pink button clicked!\n";
      m_redValue = rand() % 256;
      m_greenValue = rand() % 256;
      m_blueValue = rand() % 256;
    },
    "Random Color",
    Widget::LabelPosition::INSIDE,
    SDL_Color{ 255, 204, 255, 255 }
  ));

  m_layout.addWidget(std::make_unique<Button>(
    GEN_ID, 0, 0, 100, 50,
    [this]() {
      std::cout << "Cyan button clicked!\n";
      m_running = false; // quit
    },
    "Exit",
    Widget::LabelPosition::INSIDE,
    SDL_Color{ 102, 255, 255, 255 }
  ));

  // Sliders
  m_layout.addWidget(std::make_unique<Slider>(GEN_ID, 0, 0, 16, 100, 0, 255, m_redValue, "Red Value"));
  m_layout.addWidget(std::make_unique<Slider>(GEN_ID, 0, 0, 16, 100, 0, 255, m_greenValue, "Green Value"));
  m_layout.addWidget(std::make_unique<Slider>(GEN_ID, 0, 0, 16, 100, 0, 255, m_blueValue, "Blue Value"));

  // Text Fields
  m_layout.addWidget(std::make_unique<TextField>(GEN_ID, 0, 0, 200, 0, "Text Field 1"));
  m_layout.addWidget(std::make_unique<TextField>(GEN_ID, 0, 0, 200, 0, "Text Field 2"));
  m_layout.addWidget(std::make_unique<TextField>(GEN_ID, 0, 0, 200, 0, "Text Field 3", Widget::LabelPosition::ABOVE));

  for (auto& widget : m_layout.getWidgets()) {
    widget->setPadding(m_renderer);
  }
}

void App::imguiPrepare() { m_uiState.hotItem = 0; }

void App::imguiFinish() {
  if (m_uiState.mouseDown == 0) {
    m_uiState.activeItem = 0;
  } else if (m_uiState.activeItem == 0) {
    m_uiState.activeItem = -1;
  }
  m_uiState.keyEntered = 0;
}

void App::render() {
  m_renderer.clear({
    static_cast<unsigned char>(m_redValue),
    static_cast<unsigned char>(m_greenValue),
    static_cast<unsigned char>(m_blueValue),
    255
    });

  imguiPrepare();

  if (m_uiState.debugMode) {
    m_debugRenderer.renderDebug(m_uiState, m_layout, m_renderer);

  } else {
    auto& widgets = m_layout.getWidgets();
    std::vector<Widget*> renderOrder;

    // Update widgets first
    for (auto& widget : widgets) {
      widget->update(m_renderer, m_uiState);
      renderOrder.push_back(widget.get());
    }

    std::sort(
      renderOrder.begin(), renderOrder.end(),
      [](Widget* a, Widget* b) {
        return a->getZIndex() < b->getZIndex();
      }
    );

    for (auto& widget : renderOrder) {
      // Offset rendering based on scroll position
      SDL_Rect rect = widget->getRect();
      if (
        rect.x + rect.w + m_uiState.scrollX > 0 && rect.y + rect.h + m_uiState.scrollY > 0 &&
        rect.x + m_uiState.scrollX < m_renderer.getScreenWidth() && rect.y + m_uiState.scrollY < m_renderer.getScreenHeight()
        ) {
        widget->render(m_renderer, m_uiState);
      }
    }
  }

  if (m_uiState.isRightClickMenuOpen) {
    std::string menuOption = m_rightClickMenu.getOption(m_renderer, m_uiState);
    if (menuOption == "Properties") {
      std::cout << "Properties of Widget ID: " << m_uiState.rightClickedWidget->getId() << '\n';

    } else if (menuOption == "Delete") {
      if (m_uiState.rightClickedWidget) {
        std::cout << "Deleting Widget ID: " << m_uiState.rightClickedWidget->getId() << '\n';
        m_layout.deleteWidget(m_uiState.rightClickedWidget);
        std::cout << "Widget deleted" << std::endl;
      }
    } else if (menuOption == "Add Widget") {
      std::cout << "Adding new widget...\n";
      m_layout.addWidget(std::make_unique<Button>(GEN_ID, m_uiState.buttonRX, m_uiState.buttonRY, 100, 50));
    }
  }

  renderHelp();

  imguiFinish();

  m_renderer.present();

  SDL_Delay(10);
}

void App::renderHelp() {
  // Display debug help info widget
  SDL_Rect helpRect = { m_renderer.getScreenWidth() - 30, 10, 20, 20 };

  m_renderer.drawRect(helpRect, UTILS::COLOR::BLACK);
  m_renderer.drawTextCentered(
    "?",
    helpRect.x, helpRect.y,
    helpRect.w, helpRect.h,
    UTILS::COLOR::WHITE
  );

  // Need to adjust our mouse pos to ignore scroll distance for persistant screen items
  if (m_uiState.regionHit(helpRect.x - m_uiState.scrollX, helpRect.y - m_uiState.scrollY, helpRect.w, helpRect.h)) {
    // if help tool is hovered, render help popup
    m_renderer.drawRect(
      20, 20,
      m_renderer.getScreenWidth() - 40, m_renderer.getScreenHeight() - 40,
      UTILS::COLOR::WHITE
    );

    const int linePadding = m_renderer.getFontHeight() + 2;
    int yPos = 30;

    m_renderer.drawText("F1 - Debug mode", 30, yPos, UTILS::COLOR::BLACK);
    yPos += linePadding;
    m_renderer.drawText("   - Hover widgets for info", 30, yPos, UTILS::COLOR::BLACK);
    yPos += linePadding;
    m_renderer.drawText("   - Click and drag widgets to move grid positions", 30, yPos, UTILS::COLOR::BLACK);
    yPos += linePadding;
    yPos += linePadding;

    m_renderer.drawText("F4 - Quit", 30, yPos, UTILS::COLOR::BLACK);
    yPos += linePadding;
    yPos += linePadding;

    m_renderer.drawText("By Kobi Chambers", 30, yPos, UTILS::COLOR::BLACK);
    yPos += linePadding;

  }
}