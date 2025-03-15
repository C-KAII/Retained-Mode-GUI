#pragma once
#include "Widget.h"
#include "UIState.h"
#include "Renderer.h"
#include <vector>
#include <functional>
#include <optional>

class Graph : public Widget {
public:
  enum class GraphType { DATA_POINTS, EQUATION };

  // Ctor for datasets
  Graph(
    int id, int x, int y, int width, int height,
    SDL_Color graphColor, std::vector<std::pair<float, float>>& data
  );

  // Ctor for equations
  Graph(
    int id, int x, int y, int width, int height,
    SDL_Color graphColor, std::function<float(float)> equation
  );

  ~Graph() = default;

  void update(Renderer& renderer, UIState& uiState) override;
  void render(Renderer& renderer, const UIState& uiState) override;
  void handleCharInput(char c) override {}

  void setData(std::vector<std::pair<float, float>>& newData);
  void setEquation(std::function<float(float)> newEquation);

private:
  void drawCentralAxes(Renderer& renderer, const UIState& uiState);
  void drawAxes(Renderer& renderer, const UIState& uiState);
  void drawGraph(Renderer& renderer, const UIState& uiState);

  GraphType m_graphType;
  std::vector<std::pair<float, float>>* m_data; // For custom data
  std::optional<std::function<float(float)>> m_equation; // For equation-based graphing

  float m_xMin{ -10 };
  float m_xMax{ 10 };
  float m_yMin{ -1 };
  float m_yMax{ 1 };

  //float m_zoomFactor{ 1.0f };
};