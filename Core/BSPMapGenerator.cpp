#include "Core/BSPMapGenerator.h"
#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <random>

namespace TTOT::Core {
BSPMapGenerator::BSPMapGenerator(std::uint32_t seed, std::uint32_t width,
                                 std::uint32_t height)
    : seed(seed), width(width), height(height), rng(seed) {
  bsp.resize(width * height, 0);
}
BSPMapGenerator::~BSPMapGenerator() = default;
void BSPMapGenerator::Generate() {
  std::fill(bsp.begin(), bsp.end(), 0);
  root = std::make_unique<BSPNode>(0, 0, width, height);
  Split(root.get(), 0);
  CreateRoom(root.get());
  MergeRoom();
  ApplyCellularAutomata();
  ConnectCorridors(root.get());
  ConnectAdjacentRooms();
  FillBSPWithBorders(root.get());
}

void BSPMapGenerator::Split(BSPNode *node, int currentDepth) {
  if (!node || currentDepth >= MAX_DEPTH)
    return;
  bool splitHorizontal = (rng() % 2 == 0);
  if (node->width > node->height &&
      static_cast<double>(node->width) / node->height >= 1.25) {
    splitHorizontal = false;
  } else if (node->height > node->width &&
             static_cast<double>(node->height) / node->width >= 1.25) {
    splitHorizontal = true;
  }
  int maxLen = (splitHorizontal ? node->height : node->width) - MIN_SIZE;
  if (maxLen <= MIN_SIZE)
    return;
  std::uniform_int_distribution<int> dist(MIN_SIZE, maxLen);
  int splitPoint = dist(rng);
  if (splitHorizontal) {
    node->left =
        std::make_unique<BSPNode>(node->x, node->y, node->width, splitPoint);
    node->right = std::make_unique<BSPNode>(
        node->x, node->y + splitPoint, node->width, node->height - splitPoint);
  } else {
    node->left =
        std::make_unique<BSPNode>(node->x, node->y, splitPoint, node->height);
    node->right = std::make_unique<BSPNode>(
        node->x + splitPoint, node->y, node->width - splitPoint, node->height);
  }

  Split(node->left.get(), currentDepth + 1);
  Split(node->right.get(), currentDepth + 1);
}
void BSPMapGenerator::CreateRoom(BSPNode *node) {
  if (!node)
    return;
  if (!node->left && !node->right) {
    int padding = 2;
    if (node->width - 2 * padding < 4 || node->height - 2 * padding < 4)
      return;

    std::uniform_int_distribution<int> wDist(4, node->width - 2 * padding);
    std::uniform_int_distribution<int> hDist(4, node->height - 2 * padding);
    int roomW = wDist(rng);
    int roomH = hDist(rng);

    std::uniform_int_distribution<int> xDist(
        node->x + padding, node->x + node->width - roomW - padding);
    std::uniform_int_distribution<int> yDist(
        node->y + padding, node->y + node->height - roomH - padding);

    int roomX = xDist(rng);
    int roomY = yDist(rng);

    node->room = {roomX, roomY, roomW, roomH};
    node->hasRoom = true;

    for (int y = roomY; y < roomY + roomH; ++y) {
      for (int x = roomX; x < roomX + roomW; ++x) {
        bsp[y * width + x] = 1;
      }
    }
    return;
  }
  CreateRoom(node->left.get());
  CreateRoom(node->right.get());
}
void BSPMapGenerator::MergeRoom() {
  std::uniform_int_distribution<int> chance(0, 100);

  for (int y = 1; y < static_cast<int>(height - 1); ++y) {
    for (int x = 1; x < static_cast<int>(width - 1); ++x) {
      if (bsp[y * width + x] == 0) {
        if (bsp[y * width + (x - 1)] == 1 && bsp[y * width + (x + 1)] == 1) {
          if (chance(rng) < 30) {
            bsp[y * width + x] = 1;
          }
        } else if (bsp[(y - 1) * width + x] == 1 &&
                   bsp[(y + 1) * width + x] == 1) {
          if (chance(rng) < 30) {
            bsp[y * width + x] = 1;
          }
        }
      }
    }
  }
}
void BSPMapGenerator::ConnectCorridors(BSPNode *node) {
  if (!node || !node->left || !node->right)
    return;
  ConnectCorridors(node->left.get());
  ConnectCorridors(node->right.get());

  BSPNode *left = GetRoomNode(node->left.get());
  BSPNode *right = GetRoomNode(node->right.get());

  if (left && right) {
    int startX = left->room.x + left->room.width / 2;
    int startY = left->room.y + left->room.height / 2;
    int endX = right->room.x + right->room.width / 2;
    int endY = right->room.y + right->room.height / 2;

    int thickness = rng() % 3 + 1;

    CarveHLine(startX, endX, startY, thickness);
    CarveVLine(startY, endY, endX, thickness);
  }
}
void BSPMapGenerator::CarveHLine(int x1, int x2, int y, int thickness) {
  for (int i = 0; i < thickness; ++i) {
    int cy = y + i;
    if (cy < 0 || cy >= static_cast<int>(height))
      continue;
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
      if (x < 0 || x >= static_cast<int>(width))
        continue;
      bsp[cy * width + x] = 1;
    }
  }
}
void BSPMapGenerator::CarveVLine(int y1, int y2, int x, int thickness) {
  for (int i = 0; i < thickness; ++i) {
    int cx = x + i;
    if (cx < 0 || cx >= static_cast<int>(width))
      continue;
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
      if (y < 0 || y >= static_cast<int>(height))
        continue;
      bsp[y * width + cx] = 1;
    }
  }
}
void BSPMapGenerator::ApplyCellularAutomata() {
  std::vector<int> oldMap = bsp;
  for (int y = 1; y < static_cast<int>(height) - 1; ++y) {
    for (int x = 1; x < static_cast<int>(width) - 1; ++x) {
      int wallCount = 0;
      for (int ky = -1; ky <= 1; ++ky) {
        for (int kx = -1; kx <= 1; ++kx) {
          if (oldMap[(y + ky) * width + (x + kx)] == 0) {
            wallCount++;
          }
        }
      }
      if (oldMap[y * width + x] == 0 && wallCount < 4) {
        bsp[y * width + x] = 2;
      } else if (oldMap[y * width + x] != 0 && wallCount >= 7) {
        bsp[y * width + x] = 0;
      }
    }
  }
}
int BSPMapGenerator::CountNodes(const BSPNode *node) const {
  if (!node)
    return 0;
  return 1 + CountNodes(node->left.get()) + CountNodes(node->right.get());
}

void BSPMapGenerator::FillBSPWithBorders(const BSPNode *node) {
  if (!node)
    return;
  if (!node->left && !node->right) {
    for (int i = 0; i < node->width; ++i) {
      if (bsp[node->y * width + (node->x + i)] == 0) {
        bsp[node->y * width + (node->x + i)] = 3;
      }
      if (bsp[(node->y + node->height - 1) * width + (node->x + i)] == 0) {
        bsp[(node->y + node->height - 1) * width + (node->x + i)] = 3;
      }
    }
    for (int j = 0; j < node->height; ++j) {
      if (bsp[(node->y + j) * width + node->x] == 0) {
        bsp[(node->y + j) * width + node->x] = 3;
      }
      if (bsp[(node->y + j) * width + (node->x + node->width - 1)] == 0) {
        bsp[(node->y + j) * width + (node->x + node->width - 1)] = 3;
      }
    }
  } else {
    FillBSPWithBorders(node->left.get());
    FillBSPWithBorders(node->right.get());
  }
}

ftxui::Element BSPMapGenerator::Render() const {
  using namespace ftxui;
  std::vector<Element> rows;
  for (std::uint32_t y = 0; y < height; y += 2) {
    std::vector<Element> cells;
    for (std::uint32_t x = 0; x < width; ++x) {
      int cell_top = bsp[y * width + x];
      int cell_bottom = (y + 1 < height) ? bsp[(y + 1) * width + x] : 0;

      bool top_is_wall = (cell_top == 0 || cell_top == 3);
      bool bottom_is_wall = (cell_bottom == 0 || cell_bottom == 3);

      if (top_is_wall && bottom_is_wall) {
        cells.push_back(text("█") | color(Color::GrayDark));
      } else if (top_is_wall) {
        cells.push_back(text("▀") | color(Color::GrayDark));
      } else if (bottom_is_wall) {
        cells.push_back(text("▄") | color(Color::GrayDark));
      } else {
        cells.push_back(text(" "));
      }
    }
    rows.push_back(hbox(std::move(cells)));
  }
  return vbox(std::move(rows));
}

void BSPMapGenerator::CollectLeafRooms(BSPNode *node,
                                       std::vector<BSPNode *> &rooms) {
  if (!node)
    return;
  if (!node->left && !node->right) {
    if (node->hasRoom) {
      rooms.push_back(node);
    }
    return;
  }
  CollectLeafRooms(node->left.get(), rooms);
  CollectLeafRooms(node->right.get(), rooms);
}

void BSPMapGenerator::ConnectAdjacentRooms() {
  std::vector<BSPNode *> rooms;
  CollectLeafRooms(root.get(), rooms);

  std::uniform_int_distribution<int> chance(0, 100);

  for (size_t i = 0; i < rooms.size(); ++i) {
    for (size_t j = i + 1; j < rooms.size(); ++j) {
      BSPNode *A = rooms[i];
      BSPNode *B = rooms[j];

      int overlapYStart = std::max(A->room.y, B->room.y);
      int overlapYEnd =
          std::min(A->room.y + A->room.height, B->room.y + B->room.height);

      if (overlapYStart < overlapYEnd) {
        int gap = 0;
        int startX = 0;
        int endX = 0;
        if (A->room.x + A->room.width <= B->room.x) {
          gap = B->room.x - (A->room.x + A->room.width);
          startX = A->room.x + A->room.width - 1;
          endX = B->room.x;
        } else if (B->room.x + B->room.width <= A->room.x) {
          gap = A->room.x - (B->room.x + B->room.width);
          startX = B->room.x + B->room.width - 1;
          endX = A->room.x;
        }

        if (gap > 0 && gap <= 12) {
          if (chance(rng) < 35) {
            int overlapY = (overlapYStart + overlapYEnd - 1) / 2;
            int thickness = rng() % 2 + 1;
            CarveHLine(startX, endX, overlapY, thickness);
          }
        }
      }

      int overlapXStart = std::max(A->room.x, B->room.x);
      int overlapXEnd =
          std::min(A->room.x + A->room.width, B->room.x + B->room.width);

      if (overlapXStart < overlapXEnd) {
        int gap = 0;
        int startY = 0;
        int endY = 0;
        if (A->room.y + A->room.height <= B->room.y) {
          gap = B->room.y - (A->room.y + A->room.height);
          startY = A->room.y + A->room.height - 1;
          endY = B->room.y;
        } else if (B->room.y + B->room.height <= A->room.y) {
          gap = A->room.y - (B->room.y + B->room.height);
          startY = B->room.y + B->room.height - 1;
          endY = A->room.y;
        }

        if (gap > 0 && gap <= 12) {
          if (chance(rng) < 35) {
            int overlapX = (overlapXStart + overlapXEnd - 1) / 2;
            int thickness = rng() % 2 + 1;
            CarveVLine(startY, endY, overlapX, thickness);
          }
        }
      }
    }
  }
}

std::pair<int, int> BSPMapGenerator::GetFirstRoomCenter() const {
  struct Helper {
    static const BSPNode* findFirstRoom(const BSPNode* node) {
      if (!node) return nullptr;
      if (!node->left && !node->right) {
        if (node->hasRoom) return node;
        return nullptr;
      }
      const BSPNode* leftRes = findFirstRoom(node->left.get());
      if (leftRes) return leftRes;
      return findFirstRoom(node->right.get());
    }
  };
  const BSPNode* first = Helper::findFirstRoom(root.get());
  if (first) {
    int cx = first->room.x + first->room.width / 2;
    int cy = first->room.y + first->room.height / 2;
    return {cx, cy};
  }
  return {static_cast<int>(width) / 2, static_cast<int>(height) / 2};
}

} // namespace TTOT::Core