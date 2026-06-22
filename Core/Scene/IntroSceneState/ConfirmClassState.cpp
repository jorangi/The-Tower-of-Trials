#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/ConfirmClassState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void ConfirmClassState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void ConfirmClassState::OnExit(IntroScene& scene)
    {

    }
    void ConfirmClassState::Update(IntroScene& scene)
    {

    }
    void ConfirmClassState::HandleInput(IntroScene& scene, const Event& event)
    {
        //esc
        if(event == ftxui::Event::Escape)
        {
            scene.introContext.customClass = "";
            scene.introContext.customClassDesc = "";
            scene.ChangeState(IntroStep::CustomClass);
            return;
        }
        //좌우
        if(event == Event::ArrowLeft || event == Event::Character('a') ||
            event == Event::ArrowRight || event == Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
        //Enter
        if(event == Event::Return)
        {
            scene.ChangeState((selectedIndex == 0 ? IntroStep::StatPoint : IntroStep::CustomClass));
            return;
        }
    }
    Element ConfirmClassState::Render(IntroScene& scene)
    {
        return vbox({
                    text("사용자 정의 클래스") | bold | color(Color::Yellow),
                    separator(),
                    text(scene.introContext.customClass) | bold,
                    paragraph(scene.introContext.customClassDesc) | size(WIDTH, LESS_THAN, 80) | color(Color::GrayLight),
                    separator(),
                    text("해당 클래스로 결정하시겠습니까?"),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing),
                        text(" / "), 
                        text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                });
    }
}