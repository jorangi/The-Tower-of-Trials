#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/SelectGenderState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void SelectGenderState::OnEnter(IntroScene& scene)
    {
        scene.introContext.userGender = false;
        selectedIndex = 0;
    }
    void SelectGenderState::OnExit(IntroScene& scene){}
    void SelectGenderState::Update(IntroScene& scene){}
    void SelectGenderState::HandleInput(IntroScene& scene, const Event& event)
    {
        //Esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::ConfirmName);
            return;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            scene.introContext.userGender = (selectedIndex == 1);
            scene.ChangeState(IntroStep::personalDataConfirm);
            return;
        }
        //좌우
        if(event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a') ||
            event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
    }
    Element SelectGenderState::Render(IntroScene& scene)
    {
        return vbox({
            vbox({
                text("당신의 성별은 무엇입니까?"),
                hbox({text("남자") | (selectedIndex == 0 ? bold : nothing),
                text(" / "),
                text("여자") | (selectedIndex == 1 ? bold : nothing)
                })
            })
        });
    }
}