#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/PersonalDataConfirmState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void PersonalDataConfirmState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void PersonalDataConfirmState::OnExit(IntroScene& scene)
    {

    }
    void PersonalDataConfirmState::Update(IntroScene& scene)
    {

    }
    void PersonalDataConfirmState::HandleInput(IntroScene& scene, const Event& event)
    {
        //Esc
        if(event == Event::Escape)
        {
            scene.ChangeState(IntroStep::SelectGender);
            return;
        }
        //Enter
        if(event == Event::Return)
        {
            if(selectedIndex == 0)
                scene.ChangeState(IntroStep::CustomClass);
            else
                scene.ChangeState(IntroStep::InputName);
            return;
        }
        //좌우
        if(event == Event::ArrowLeft || event == Event::Character('a') ||
            event == Event::ArrowRight || event == Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
    }
    Element PersonalDataConfirmState::Render(IntroScene& scene)
    {
        return vbox({
            hbox({text(std::string("당신의 이름은")),
                                    text(scene.introContext.userName) | color(Color::Green),
                                    text(std::string("입니다. \n 당신의 성별은")),
                                    text(scene.introContext.userGender ? "여자" : "남자") | color(Color::Green),
                                    text(std::string("입니다."))
            }),
            separator(),
            hbox({
                    text("예") | (selectedIndex == 0 ? bold : nothing),
                    text(" / "),
                    text("아니오") | (selectedIndex == 1 ? bold : nothing)
                })
        });
    }
}