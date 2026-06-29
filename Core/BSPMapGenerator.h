#pragma once
#include <cstdint>
#include <ftxui/dom/elements.hpp>
#include <memory>
#include <random>
#include <vector>

namespace TTOT::Core {
class BSPMapGenerator {
private:
  std::vector<int> bsp;
  std::uint32_t seed, width, height;
  struct Rectangle {
    int x, y, width, height;
  };
  struct BSPNode {
    BSPNode(int x, int y, int width, int height)
        : left(nullptr), right(nullptr), x(x), y(y), width(width),
          height(height), size(width * height) {}
    std::unique_ptr<BSPNode> left;
    std::unique_ptr<BSPNode> right;
    int x, y;
    int width, height = 0;
    int size = width * height;

    Rectangle room;
    bool hasRoom = false;
  };
  std::unique_ptr<BSPNode> root = nullptr;
  const int MIN_SIZE = 15;
  const int MAX_DEPTH = 6;
  std::mt19937 rng;

  void Split(BSPNode *node, int currentDepth);
  void CreateRoom(BSPNode *node);
  void MergeRoom();
  BSPNode *GetRoomNode(BSPNode *node) {
    if (!node)
      return nullptr;
    if (node->hasRoom)
      return node;
    auto *left = GetRoomNode(node->left.get());
    if (left)
      return left;
    return GetRoomNode(node->right.get());
  }
  void ConnectCorridors(BSPNode *node);
  void CollectLeafRooms(BSPNode *node, std::vector<BSPNode *> &rooms);
  void ConnectAdjacentRooms();
  void CarveHLine(int x1, int x2, int y, int thickness);
  void CarveVLine(int y1, int y2, int x, int thickness);
  void ApplyCellularAutomata();
  int CountNodes(const BSPNode *node) const;
  void FillBSPWithBorders(const BSPNode *node);

public:
  BSPMapGenerator(std::uint32_t seed, std::uint32_t width,
                  std::uint32_t height);
  ~BSPMapGenerator();
  void Generate();
  ftxui::Element Render() const;
  std::pair<int, int> GetFirstRoomCenter() const;

  const std::vector<int> &GetBSP() const { return bsp; }
  std::uint32_t GetWidth() const { return width; }
  std::uint32_t GetHeight() const { return height; }
  int GetNodeCount() const { return CountNodes(root.get()); }
};
} // namespace TTOT::Core