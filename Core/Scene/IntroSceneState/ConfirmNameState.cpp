#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/ConfirmNameState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void ConfirmNameState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void ConfirmNameState::OnExit(IntroScene& scene)
    {

    }
    void ConfirmNameState::Update(IntroScene& scene)
    {

    }
    void ConfirmNameState::HandleInput(IntroScene& scene, const ftxui::Event& event)
    {
        //Esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::InputName);
            return;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            if(selectedIndex == 0)
            {
                scene.ChangeState(IntroStep::SelectGender);
                return;
            }
            else
            {
                scene.ChangeState(IntroStep::InputName);
                return;
            }
        }
        //좌우
        if(event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a') ||
            event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
    }
    ftxui::Element ConfirmNameState::Render(IntroScene& scene)
    {
        int i = scene.introContext.savedName[scene.introContext.userName];
        if(i > 6)
        {
            scene.ChangeState(IntroStep::SelectGender);
            return text("");
        }
        else
        {
            Elements el;
            if(i < 2) 
                el.push_back(text("▶ '" + scene.introContext.userName + "' 이(가) 당신의 이름이 맞습니까?"));
            if(i == 2)
                el.push_back(text("▶ " + scene.introContext.userName +" 맞는거죠?"));
            if(i == 3)
                el.push_back(text("▶ ...? 아까 " + scene.introContext.userName + "은 아니라고 하지 않았어요? 본인 이름 맞아요?"));
            if(i == 4)
                el.push_back(text("▶ 장난하는거에요? " + scene.introContext.userName + "이 이름 맞아요?"));
            if(i == 5)
                el.push_back(text("▶ 재미 없거든요... " + scene.introContext.userName + " 확실한거죠?"));
            if(i == 6)
                el.push_back(text("▶ 이제 더 이상 안 받아줄거에요.. " + scene.introContext.userName + " 맞는지 마지막으로 확인합니다."));
            
            el.push_back(
                hbox({text("네") | (selectedIndex == 0 ? bold : nothing),
                        text(" / "),
                        text("아니오") | (selectedIndex == 1 ? bold : nothing)
                })
            );
            return vbox(std::move(el)) | border;
        }
    }
}