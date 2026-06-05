#include "Core/Scene/TitleScene.h"
#include <Utility/ScreenUtility.h>
#include <windows.h>
#include "Core/Event/SoundEvent.h"
#include "Core/GameContext.h"
#include "Core/Event/GameLoopEvent.h"
#include "Core/Event/SceneChangeEvent.h"
namespace TTOT::Core::Scenes
{
    TTOT::Core::Scenes::TitleScene::TitleScene(TTOT::Core::GameContext& context) : SceneBase(context) {}
    void TTOT::Core::Scenes::TitleScene::OnEnter()
    {
        context.eventBus.Publish(TTOT::Core::Events::SoundEvent{"BGM_Title"});
    }
    void TTOT::Core::Scenes::TitleScene::OnExit(){}
    void TTOT::Core::Scenes::TitleScene::Update(){}
    void TTOT::Core::Scenes::TitleScene::HandleInput(const ftxui::Event& event)
    {
        if(event == ftxui::Event::ArrowDown)
        {
            selectedIndex = (selectedIndex + 1) % 4;
            return;
        }
        else if(event == ftxui::Event::ArrowUp)
        {
            selectedIndex = (selectedIndex - 1 + 4) % 4;
            return;
        }
        if(event == ftxui::Event::Character('1') || (event == ftxui::Event::Return && selectedIndex == 0))
        {
            outputText = saveFile ? "불러오기 선택됨" : "세이브 파일이 없습니다.";
        }
        else if(event == ftxui::Event::Character('2') || (event == ftxui::Event::Return && selectedIndex == 1))
        {
            outputText = "새 게임 선택됨";
            context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{2});
            return;
        }
        else if(event == ftxui::Event::Character('3') || (event == ftxui::Event::Return && selectedIndex == 2))
        {
            outputText = "설정 선택됨";
            context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{1});
            return;
        }
        else if(event == ftxui::Event::Character('4') || event == ftxui::Event::Escape || (event == ftxui::Event::Return && selectedIndex == 3))
        {
            outputText = "종료 선택됨";
            context.eventBus.Publish(TTOT::Core::Events::GameLoopEvent{TTOT::Core::Events::GameLoopAction::Exitgame});
            return;
        }
        else
        {
            InputError("잘못된 입력입니다.");
        }
    }
    ftxui::Element TTOT::Core::Scenes::TitleScene::Render()
    {
        using namespace ftxui;
        return vbox({
            vbox({
                text("The Tower of Trials") | bold | color(Color::Yellow) | hcenter
            }) | border,
            vbox({
                text("1. 불러오기") | color(Color::Cyan) | (selectedIndex == 0 ? bold : nothing),
                text("2. 새 게임") | color(Color::Green) | (selectedIndex == 1 ? bold : nothing),
                text("3. 설정") | color(Color::White) | (selectedIndex == 2 ? bold : nothing),
                text("4. 종료") | color(Color::Red) | (selectedIndex == 3 ? bold : nothing)
            }) | hcenter,
            separator(),
            vbox({
                text(outputText) | color(Color::GrayLight) | hcenter
            }) | hcenter
        }) | border;
    }
}