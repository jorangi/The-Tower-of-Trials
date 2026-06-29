#include "Core/Scene/IntroSceneState/ProposalState.h"
#include "Core/Event/InjectPurposeEvent.h"
#include "Core/Event/SaveRequestEvent.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Scene/IntroScene.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState {
void ProposalState::OnEnter(IntroScene &scene) {}
void ProposalState::OnExit(IntroScene &scene) {}
void ProposalState::Update(IntroScene &scene) {}
void ProposalState::HandleInput(IntroScene &scene, const ftxui::Event &event) {
  // Enter
  if (event == ftxui::Event::Return) {
    scene.context.eventBus.Publish(Events::InjectPurposeEvent{
        scene.introContext.adventurePurpose, scene.introContext.guidance,
        scene.introContext.questDataJson});
    scene.context.eventBus.Publish(Events::SaveRequestEvent{1});
    scene.context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{3});
    return;
  }
}
Element ProposalState::Render(IntroScene &scene) {
  return vbox(
      {text("당신에게 탑의 여신의 목소리가 들립니다.") | bold |
           color(Color::Yellow),
       separator(),
       paragraph(scene.introContext.guidance) | size(WIDTH, LESS_THAN, 80) |
           color(Color::GrayLight),
       separator(),
       text("당신의 선택에 따라 탑 안에서 무한한 가능성이 열릴 것입니다."),
       text("고개를 끄덕인다") | bold});
}
} // namespace TTOT::Core::Scenes::IntroSceneState