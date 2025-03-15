#include "Graph.h"

Graph::Graph(
  int id, int x, int y, int width, int height,
  SDL_Color graphColor, std::vector<std::pair<float, float>>& data
) : m_data(&data), m_equation(nullptr), m_graphType(GraphType::DATA_POINTS) {
  m_id = id;
  m_widgetType = WidgetType::GRAPH;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_color = graphColor;
}

Graph::Graph(
  int id, int x, int y, int width, int height,
  SDL_Color graphColor, std::function<float(float)> equation
) : m_equation(equation), m_data(nullptr), m_graphType(GraphType::EQUATION) {
  m_id = id;
  m_widgetType = WidgetType::GRAPH;
  m_x = x;
  m_y = y;
  m_width = width;
  m_height = height;
  m_color = graphColor;
}

void Graph::update(Renderer& renderer, UIState& uiState) {
  if (uiState.regionHit(m_x, m_y, m_width, m_height)) {
    uiState.hotItem = m_id;
  }
}

void Graph::render(Renderer& renderer, const UIState& uiState) {
  renderer.drawRect(m_x + uiState.scrollX, m_y + uiState.scrollY, m_width, m_height, UTILS::COLOR::VERY_DARK_GREY);

  if (m_graphType == GraphType::DATA_POINTS) {
    drawAxes(renderer, uiState);
  } else {
    drawCentralAxes(renderer, uiState);
  }

  drawGraph(renderer, uiState);
}

void Graph::setData(std::vector<std::pair<float, float>>& newData) {
  if (m_graphType == GraphType::DATA_POINTS) {
    m_data = &newData;
  }
}

void Graph::setEquation(std::function<float(float)> newEquation) {
  if (m_graphType == GraphType::EQUATION) {
    m_equation = newEquation;
  }
}

// private:
  
void Graph::drawCentralAxes(Renderer& renderer, const UIState& uiState) {
  int centerX = m_x + (m_width / 2) + uiState.scrollX;
  int centerY = m_y + (m_height / 2) + uiState.scrollY;
  // X-axis
  renderer.drawLine(m_x + uiState.scrollX, centerY, m_x + m_width + uiState.scrollX, centerY, UTILS::COLOR::WHITE);
  // Y-axis
  renderer.drawLine(centerX, m_y + uiState.scrollY, centerX, m_y + m_height + uiState.scrollY, UTILS::COLOR::WHITE);
}

void Graph::drawAxes(Renderer& renderer, const UIState& uiState) {
  // X-axis
  renderer.drawLine(m_x + uiState.scrollX, m_y + m_height + uiState.scrollY, m_x + m_width + uiState.scrollX, m_y + m_height + uiState.scrollY, UTILS::COLOR::WHITE);
  // Y-axis
  renderer.drawLine(m_x + uiState.scrollX, m_y + uiState.scrollY, m_x + uiState.scrollX, m_y + m_height + uiState.scrollY, UTILS::COLOR::WHITE);
}
  
void Graph::drawGraph(Renderer& renderer, const UIState& uiState) {
  if (m_graphType == GraphType::EQUATION && m_equation.has_value()) {
    for (int i = 0; i < m_width; i++) {
      float x = m_xMin + (i / static_cast<float>(m_width)) * (m_xMax - m_xMin);
      float y = m_equation.value()(x);

      int screenX = m_x + i;
      int screenY = m_y + m_height - static_cast<int>((y - m_yMin) / (m_yMax - m_yMin) * m_height);

      if (screenY > m_y && screenY < m_y + m_height) {
        renderer.drawPixel(screenX + uiState.scrollX, screenY + uiState.scrollY, m_color);
      }
    }
  } else if (m_graphType == GraphType::DATA_POINTS && m_data && !m_data->empty()) {
    // Determine min/max dynamically
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& point : *m_data) {
      minX = std::min(minX, point.first);
      maxX = std::max(maxX, point.first);
      minY = std::min(minY, point.second);
      maxY = std::max(maxY, point.second);
    }

    // Avoid divide by zero issues
    if (maxX == minX) maxX += 1.0f;
    if (maxY == minY) maxY += 1.0f;

    // Data-based graph rendering
    int dataSize = static_cast<int>(m_data->size());
    if (dataSize > 1) {
      for (int i = 0; i < dataSize - 1; i++) {
        float normalizedX1 = (m_data->at(i).first - minX) / (maxX - minX);
        float normalizedX2 = (m_data->at(i + 1).first - minX) / (maxX - minX);
        float normalizedY1 = (m_data->at(i).second - minY) / (maxY - minY);
        float normalizedY2 = (m_data->at(i + 1).second - minY) / (maxY - minY);

        int x1 = m_x + static_cast<int>(normalizedX1 * m_width);
        int y1 = m_y + m_height - static_cast<int>(normalizedY1 * m_height);
        int x2 = m_x + static_cast<int>(normalizedX2 * m_width);
        int y2 = m_y + m_height - static_cast<int>(normalizedY2 * m_height);

        renderer.drawLine(x1 + uiState.scrollX, y1 + uiState.scrollY, x2 + uiState.scrollX, y2 + uiState.scrollY, m_color);
      }
    }
  }
}