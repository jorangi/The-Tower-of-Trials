#include "Core/BSPMapGenerator.h"
#include <cassert>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace TTOT::Core;

void Test_BSPMapGenerator_Initialization() {
  cout << "Testing BSPMapGenerator Initialization..." << endl;

  std::uint32_t seed = 42;
  std::uint32_t width = 60;
  std::uint32_t height = 40;

  BSPMapGenerator generator(seed, width, height);

  assert(generator.GetWidth() == width);
  assert(generator.GetHeight() == height);
  assert(generator.GetBSP().size() == width * height);
  assert(generator.GetNodeCount() == 0);

  for (int cell : generator.GetBSP()) {
    assert(cell == 0);
  }

  cout << "-> Initialization Test Passed!" << endl;
}

void Test_BSPMapGenerator_GenerationAndVisual() {
  cout << "Testing BSPMapGenerator Generation and Visual Display..." << endl;

  std::uint32_t seed = 30131209;
  std::uint32_t width = 100;
  std::uint32_t height = 100;
  std::cin >> seed;
  std::cin >> width;
  std::cin >> height;

  BSPMapGenerator generator(seed, width, height);
  generator.Generate();

  int nodeCount = generator.GetNodeCount();
  cout << "Generated BSP tree contains " << nodeCount << " nodes." << endl;
  assert(nodeCount > 0);

  // Verify that rooms/corridors (value 1) exist
  bool hasRoomsOrCorridors = false;
  // Verify that Cellular Automata floors (value 2) exist
  bool hasCAFloors = false;
  // Verify that walls (value 0) exist
  bool hasWalls = false;

  const auto &bsp = generator.GetBSP();
  for (int cell : bsp) {
    if (cell == 0)
      hasWalls = true;
    else if (cell == 1)
      hasRoomsOrCorridors = true;
    else if (cell == 2)
      hasCAFloors = true;
  }

  assert(hasWalls);
  assert(hasRoomsOrCorridors);
  cout << "-> Components Verification Passed! (Walls & Rooms/Corridors "
          "generated successfully)"
       << endl;
  if (hasCAFloors) {
    cout << "-> Cellular Automata Verification Passed! (CA Floor cells present)"
         << endl;
  } else {
    cout << "-> Cellular Automata Warning: No CA Floor cells found under "
            "current parameters (normal depending on rng/seed)."
         << endl;
  }

  cout << "\n============================================ " << endl;
  cout << "       Visualizing BSP Map (80x30)           " << endl;
  cout << "============================================ " << endl;
  cout << " Legend: " << endl;
  cout << "   '#' : Wall" << endl;
  cout << "   '.' : Room / Corridor Floor" << endl;
  cout << "   ' ' : Cellular Automata Floor (Open Space)" << endl;
  cout << "   '/' : BSP Partition Border (overlaid on walls)" << endl;
  cout << "--------------------------------------------" << endl;

  generator.Visualize();

  cout << "============================================\n" << endl;
}

void Test_BSPMapGenerator_Determinism() {
  cout << "Testing BSPMapGenerator Determinism..." << endl;

  std::uint32_t seed = 98765;
  std::uint32_t width = 80;
  std::uint32_t height = 60;

  BSPMapGenerator gen1(seed, width, height);
  gen1.Generate();
  int count1 = gen1.GetNodeCount();
  auto bsp1 = gen1.GetBSP();

  BSPMapGenerator gen2(seed, width, height);
  gen2.Generate();
  int count2 = gen2.GetNodeCount();
  auto bsp2 = gen2.GetBSP();

  assert(count1 == count2);
  assert(bsp1 == bsp2);
  cout << "-> Determinism Test Passed! (BSP structures and sizes match "
          "perfectly)"
       << endl;
}

int main() {
  SetConsoleOutputCP(CP_UTF8);
  cout << "============================================" << endl;
  cout << "      BSPMapGenerator Unit Tests            " << endl;
  cout << "============================================" << endl;

  Test_BSPMapGenerator_Initialization();
  Test_BSPMapGenerator_GenerationAndVisual();
  Test_BSPMapGenerator_Determinism();

  cout << "============================================" << endl;
  cout << "      All BSPMapGenerator Tests Passed!     " << endl;
  cout << "============================================" << endl;

  return 0;
}
