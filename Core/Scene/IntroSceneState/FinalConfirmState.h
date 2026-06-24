#pragma once
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"

namespace TTOT::Core::Scenes::IntroSceneState {
class FinalConfirmState : public IIntroSceneState {
private:
  int selectedIndex = 0;
  int classHP;
  int classMP;
  int classSTR;
  int classDEX;
  int classINT;
  int classWIS;
  int classCHA;
  int classDEF;
  int classSPD;
  float classHPMul;
  float classMPMul;
  float classStrMul;
  float classDexMul;
  float classIntMul;
  float classWisMul;
  float classChaMul;
  float classDefMul;
  float classSpdMul;

public:
  virtual void OnEnter(TTOT::Core::Scenes::IntroScene &scene) override;
  virtual void OnExit(TTOT::Core::Scenes::IntroScene &scene) override;
  virtual void Update(TTOT::Core::Scenes::IntroScene &scene) override;
  virtual void HandleInput(TTOT::Core::Scenes::IntroScene &scene,
                           const ftxui::Event &event) override;
  virtual ftxui::Element Render(TTOT::Core::Scenes::IntroScene &scene) override;
};
} // namespace TTOT::Core::Scenes::IntroSceneState