#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/CustomClassState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void CustomClassState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void CustomClassState::OnExit(IntroScene& scene)
    {

    }
    void CustomClassState::Update(IntroScene& scene)
    {

    }
    void CustomClassState::HandleInput(IntroScene& scene, const Event& event)
    {
        //Esc
        if(event == Event::Escape)
        {
            scene.ChangeState(IntroStep::personalDataConfirm);
            return;
        }
        //Enter
        if(event == Event::Return)
        {
            if(selectedIndex == 0)
                scene.ChangeState(IntroStep::InputCustomClass);
            else
                scene.ChangeState(IntroStep::SelectClass);
            return;
        }
        //좌우
        if(event == Event::ArrowLeft || event == Event::Character('a') ||
            event == Event::ArrowRight || event == Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
    }
    Element CustomClassState::Render(IntroScene& scene)
    {
        return vbox({text("사용자 정의 직업(클래스)를 사용하시겠습니까?"), 
                hbox({text("네") | (selectedIndex == 0 ? bold : nothing),
                            text(" / "),
                            text("아니오") | (selectedIndex == 1 ? bold : nothing)
                })
            });
    }
}