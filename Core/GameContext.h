#pragma once
#include "Core/EventBus.h"
#include "Network/ConnGemini.h"
#include "Utility/ImageRenderer.h"

namespace TTOT::Core {
struct PlayerBaseInfo {
  bool gender;
  std::string name;
  std::string className;
  std::string classDesc;
  std::string AdventurePurpose;
  std::string guidance;
  std::string questDataJson;
  int currentFloor = 1;
  unsigned int mapSeed = 0;
  int playerX = -1;
  int playerY = -1;
};

struct GameContext {
  PlayerBaseInfo playerBaseInfo;
  TTOT::Core::EventBus &eventBus;
  TTOT::Utilities::ImageRenderer &imageRenderer;
  TTOT::Network::ConnGemini &gemini;
  int settingReturnSceneId = 0;
  GameContext(EventBus &eb, Utilities::ImageRenderer &ir,
              Network::ConnGemini &gm)
      : eventBus(eb), imageRenderer(ir), gemini(gm) {}
};
} // namespace TTOT::Core