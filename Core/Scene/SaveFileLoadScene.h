#pragma once
#include "Core/Scene/SceneBase.h"
#include <array>
#include <string>

namespace TTOT::Core::Scenes {
struct SaveSlotInfo {
  bool exists = false;
  std::string saveTime;
  std::string playerName;
  std::string playerClass;
  int currentFloor = 1;
};

class SaveFileLoadScene : public SceneBase {
private:
  std::array<SaveSlotInfo, 10> saveSlots;

public:
  SaveFileLoadScene(TTOT::Core::GameContext &context);
  void OnEnter() override;
  void OnExit() override;
  void Update() override;
  void HandleInput(const ftxui::Event &event) override;
  ftxui::Element Render() override;
};
} // namespace TTOT::Core::Scenes
