#include "Utility/Camera.h"
#include <algorithm>

namespace TTOT::Utilities {

Camera::Camera(int viewportWidth, int viewportHeight, int zoom, bool gender)
    : viewportWidth(viewportWidth), viewportHeight(viewportHeight),
      zoomFactor(zoom), gender(gender) {

  // Auto-subscribe targeting coordinates when OnPositionUpdated is triggered
  OnPositionUpdated.AddListener(
      [this](int x, int y) { this->SetTargetPosition(x, y); });
}

void Camera::SetTargetPosition(int x, int y) {
  centerX = x;
  centerY = y;
}

void Camera::SetZoom(int zoom) { zoomFactor = std::max(1, zoom); }

int Camera::GetZoom() const { return zoomFactor; }

void Camera::SetViewportSize(int w, int h) {
  viewportWidth = std::max(1, w);
  viewportHeight = std::max(1, h);
}

ftxui::Element Camera::Apply(ftxui::Element mapElement) const {
  using namespace ftxui;
  // FTXUI framework-based centering: wrap inside a frame and apply size limits.
  // If the map elements have a focused element (e.g. the player), ftxui will
  // scroll/center it automatically.
  return mapElement | size(WIDTH, EQUAL, viewportWidth) |
         size(HEIGHT, EQUAL, viewportHeight) | frame | border;
}

ftxui::Element
Camera::RenderViewport(const std::vector<int> &grid, int mapWidth,
                       int mapHeight, int playerX, int playerY,
                       const std::vector<RenderableEntity> &entities) const {
  using namespace ftxui;

  // Viewport dimensions (adjusted by zoom level)
  // Higher zoom levels mean a smaller area of the map is visible, making each
  // tile look larger.
  int viewW = std::max(2, viewportWidth / zoomFactor);
  int viewH = std::max(2, viewportHeight / zoomFactor);

  // Calculate start positions to center on (playerX, playerY)
  int startX = playerX - viewW / 2;
  int startY = playerY - viewH / 2;

  // Clamp start positions to map bounds
  startX = std::max(0, std::min(startX, mapWidth - viewW));
  startY = std::max(0, std::min(startY, mapHeight - viewH));

  std::vector<Element> rows;
  for (int y = startY; y < startY + viewH; ++y) {
    // We can have multiple sub-rows per grid row when zoomFactor > 1
    std::vector<std::vector<Element>> subRows(zoomFactor);

    for (int x = startX; x < startX + viewW; ++x) {
      if (x == playerX && y == playerY) {
        // Player Tile: Center a single emoji (width 2) in a block of visual
        // width 2 * zoomFactor
        int paddingSize = zoomFactor - 1;
        std::string spaces(paddingSize, ' ');

        for (int r = 0; r < zoomFactor; ++r) {
          std::string lineStr = "";
          if (r == zoomFactor / 2) {
            lineStr = spaces + (gender ? "👩🏻" : "🧑🏻") + spaces;
          } else {
            lineStr = std::string(2 * zoomFactor, ' ');
          }
          subRows[r].push_back(text(lineStr) | color(Color::Green) | bold);
        }
      } else {
        // Check if there is another entity at (x, y)
        const RenderableEntity *activeEntity = nullptr;
        for (const auto &ent : entities) {
          if (ent.x == x && ent.y == y) {
            activeEntity = &ent;
            break;
          }
        }

        if (activeEntity) {
          // Entity Tile: Center the entity symbol in a block of visual width 2
          // * zoomFactor. Multi-byte character strings (length > 1) are treated
          // as wide characters (width 2). Single-byte ASCII strings (length ==
          // 1) are treated as narrow characters (width 1).
          bool isWide = (activeEntity->symbol.length() > 1);
          int paddingLeft = zoomFactor - 1;
          int paddingRight = isWide ? (zoomFactor - 1) : zoomFactor;

          std::string leftSpaces(paddingLeft, ' ');
          std::string rightSpaces(paddingRight, ' ');

          for (int r = 0; r < zoomFactor; ++r) {
            std::string lineStr = "";
            if (r == zoomFactor / 2) {
              lineStr = leftSpaces + activeEntity->symbol + rightSpaces;
            } else {
              lineStr = std::string(2 * zoomFactor, ' ');
            }

            Element cell = text(lineStr) | color(activeEntity->color);
            if (activeEntity->isBold) {
              cell = cell | bold;
            }
            subRows[r].push_back(cell);
          }
        } else {
          // Regular Map Tile (Wall / Floor)
          int cellValue = grid[y * mapWidth + x];
          bool isWall = (cellValue == 0 || cellValue == 3);

          // Construct horizontal block
          std::string tileStr = "";
          for (int z = 0; z < zoomFactor; ++z) {
            tileStr += isWall ? "██" : "  ";
          }

          for (int r = 0; r < zoomFactor; ++r) {
            Element tileElement = text(tileStr);
            if (isWall) {
              tileElement = tileElement | color(Color::GrayDark);
            }
            subRows[r].push_back(tileElement);
          }
        }
      }
    }

    for (int r = 0; r < zoomFactor; ++r) {
      rows.push_back(hbox(std::move(subRows[r])));
    }
  }

  return vbox(std::move(rows)) | border;
}

} // namespace TTOT::Utilities
