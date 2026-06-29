#pragma once
#include "Core/BSPMapGenerator.h"
#include "Core/Scene/SceneBase.h"
#include "UI/UserInformation.h"
#include "Utility/Camera.h"
#include <memory>
#include <vector>
#include <string>

namespace TTOT::Core::Scenes {
class FieldScene : public SceneBase {
private:
  uint32_t seed, width, height;
  std::unique_ptr<TTOT::Core::BSPMapGenerator> mapGenerator;
  std::unique_ptr<TTOT::Utilities::Camera> camera;
  std::unique_ptr<TTOT::UI::UserInformation> userInfoUI;
  int playerX = 0;
  int playerY = 0;
  uint32_t saveCompleteSubId = 0;
  std::vector<std::string> gameLogs;
  void AddLog(const std::string& msg);
  std::string GetCurrentTimeStr();

public:
  FieldScene(TTOT::Core::GameContext &context);
  void OnEnter() override;
  void OnExit() override;
  void Update() override;
  void HandleInput(const ftxui::Event &event) override;
  ftxui::Element Render() override;
};
} // namespace TTOT::Core::Scenes