#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/AmendDataState.h"

using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;
using namespace ftxui;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void AmendDataState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void AmendDataState::OnExit(IntroScene& scene)
    {
    }
    void AmendDataState::Update(IntroScene& scene)
    {

    }
    void AmendDataState::HandleInput(IntroScene& scene, const ftxui::Event& event) 
    {
        //Esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::finalConfirm);
            return;
        }
        //좌우
        if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    
            selectedIndex = (selectedIndex - 1 + 4) % 4;
        if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  
            selectedIndex = (selectedIndex + 1) % 4;
        //Enter or 1234
        if(event == ftxui::Event::Character('1') || (event == ftxui::Event::Return && selectedIndex == 0))
        {
            scene.ChangeState(IntroStep::InputName);
            return;
        }
        else if(event == ftxui::Event::Character('2') || (event == ftxui::Event::Return && selectedIndex == 1))
        {
            scene.ChangeState(IntroStep::SelectGender);
            return;
        }
        else if(event == ftxui::Event::Character('3') || (event == ftxui::Event::Return && selectedIndex == 2))
        {
            scene.ChangeState(IntroStep::CustomClass);
            return;
        }
        else if(event == ftxui::Event::Character('4') || (event == ftxui::Event::Return && selectedIndex == 3))
        {
            scene.ChangeState(IntroStep::StatPoint);
            return;
        }
    }
    ftxui::Element AmendDataState::Render(IntroScene& scene)
    {
        return(
            vbox({
                    text("이동을 원하는 번호를 선택하세요."),
                    separator(),
                    text("1. 이름") | color(Color::Aquamarine1) | (selectedIndex == 0 ? bold : nothing),
                    text("2. 성별") | color(Color::Yellow) | (selectedIndex == 1 ? bold : nothing),
                    text("3. 클래스") | color(Color::Violet) | (selectedIndex == 2 ? bold : nothing),
                    text("4. 스탯") | color(Color::Orchid1) | (selectedIndex == 3 ? bold : nothing)
                })
        );
    }
}