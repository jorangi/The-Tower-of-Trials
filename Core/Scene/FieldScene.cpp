#include "Core/Scene/FieldScene.h"
#include "Core/Event/PlayerGetEvent.h"
#include "Entity/Player.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

using namespace ftxui;
namespace TTOT::Core::Scenes
{
    FieldScene::FieldScene(TTOT::Core::GameContext& context) : SceneBase(context) {}
    void FieldScene::OnEnter()
    {
        context.eventBus.Publish(TTOT::Core::Events::PlayerGetEvent{
            [this](const TTOT::Entities::Player& playerStates)
            {
                userInfoUI = std::make_unique<TTOT::UI::UserInformation>(playerStates);
            }
        });
    }
    void FieldScene::OnExit(){}
    void FieldScene::Update(){}
    void FieldScene::HandleInput(const ftxui::Event& event)
    {

    }
    ftxui::Element FieldScene::Render()
    {
        return hbox({
            separator(),
            userInfoUI->Render()
        }) | border;
    }
}