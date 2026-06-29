#pragma once
#include "Utility/Action.h"
#include <ftxui/dom/elements.hpp>
#include <vector>

namespace TTOT::Utilities {

struct RenderableEntity {
  int x;
  int y;
  std::string symbol;
  ftxui::Color color;
  bool isBold = false;
};

class Camera {
private:
  int centerX = 0;
  int centerY = 0;
  int zoomFactor = 1;      // Zoom level (e.g., 1 for 1x, 2 for 2x tile scaling)
  int viewportWidth = 30;  // Viewport width in grid cells
  int viewportHeight = 20; // Viewport height in grid cells
  bool gender;

public:
  // Action to subscribe to position updates (e.g., player's coordinates)
  Action<int, int> OnPositionUpdated;

  Camera(int viewportWidth = 30, int viewportHeight = 20, int zoom = 1,
         bool gender = false);
  ~Camera() = default;

  // Getters and Setters
  void SetTargetPosition(int x, int y);
  void SetZoom(int zoom);
  int GetZoom() const;
  void SetViewportSize(int w, int h);

  // Render method: Takes full map element or grid, and returns the
  // cropped/zoomed FTXUI Element
  ftxui::Element Apply(ftxui::Element mapElement) const;

  // Grid-based Render method (Recommended for performance & true tile-based
  // zoom) Takes the raw map data and generates the zoomed viewport element
  ftxui::Element RenderViewport(
      const std::vector<int> &grid, int mapWidth, int mapHeight, int playerX,
      int playerY,
      const std::vector<RenderableEntity> &entities = {}) const;
};

} // namespace TTOT::Utilities
