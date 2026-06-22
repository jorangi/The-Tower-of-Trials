#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/ConfirmStatsState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void ConfirmStatsState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void ConfirmStatsState::OnExit(IntroScene& scene)
    {

    }
    void ConfirmStatsState::Update(IntroScene& scene)
    {

    }
    void ConfirmStatsState::HandleInput(IntroScene& scene, const Event& event)
    {
        //esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::StatPoint);
            return;
        }
        //좌우
        if(event == Event::ArrowLeft || event == Event::Character('a') ||
            event == Event::ArrowRight || event == Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            scene.ChangeState((selectedIndex == 0 ? IntroStep::finalConfirm : IntroStep::StatPoint));
        }
    }
    Element ConfirmStatsState::Render(IntroScene& scene)
    {
        return vbox({
                    text("당신의 스탯을 확정하겠습니까?"),
                    separator(),
                    text("STR: " + std::to_string(scene.introContext.statPoints[0])),
                    text("DEX: " + std::to_string(scene.introContext.statPoints[1])),
                    text("INT: " + std::to_string(scene.introContext.statPoints[2])),
                    text("WIS: " + std::to_string(scene.introContext.statPoints[3])),
                    text("CHA: " + std::to_string(scene.introContext.statPoints[4])),
                    text("DEF: " + std::to_string(scene.introContext.statPoints[5])),
                    text("SPD: " + std::to_string(scene.introContext.statPoints[6])),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing), 
                        text(" / "), 
                        text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                });
    }
}