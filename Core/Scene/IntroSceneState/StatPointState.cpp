#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/StatPointState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void StatPointState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
        scene.introContext.statPoint = 10;
        scene.introContext.statPoints = {0, 0, 0, 0, 0, 0, 0};
    }
    void StatPointState::OnExit(IntroScene& scene)
    {

    }
    void StatPointState::Update(IntroScene& scene)
    {

    }
    void StatPointState::HandleInput(IntroScene& scene, const Event& event)
    {
        //esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::CustomClass);
            return;
        }
        //스탯 선택 및 조정
        if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    
            selectedIndex = (selectedIndex - 1 + 7) % 7;
        if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  
            selectedIndex = (selectedIndex + 1) % 7;
        if((event == ftxui::Event::ArrowLeft || 
            event == ftxui::Event::Character('a')) && 
            scene.introContext.statPoints[selectedIndex] > 0)
        {
            scene.introContext.statPoint++;
            scene.introContext.statPoints[selectedIndex]--;
        }
        if((event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d')) && scene.introContext.statPoint > 0)
        {
            scene.introContext.statPoint--;
            scene.introContext.statPoints[selectedIndex]++;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            if(scene.introContext.statPoint > 0) return;
            scene.ChangeState(IntroStep::ConfirmStats);
            return;
        }
    }
    Element StatPointState::Render(IntroScene& scene)
    {
        std::string strBar, dexBar, intBar, wisBar, chaBar, defBar, spdBar;
        for(int i = 0; i < 10; i++)
        {
            strBar += scene.introContext.statPoints[0] > i ? "■" : "□";
            dexBar += scene.introContext.statPoints[1] > i ? "■" : "□";                
            intBar += scene.introContext.statPoints[2] > i ? "■" : "□";            
            wisBar += scene.introContext.statPoints[3] > i ? "■" : "□";                
            chaBar += scene.introContext.statPoints[4] > i ? "■" : "□";                
            defBar += scene.introContext.statPoints[5] > i ? "■" : "□";                
            spdBar += scene.introContext.statPoints[6] > i ? "■" : "□";
        }
        return vbox({
                    text(std::string("능력치 포인트 투자 (") + std::to_string(scene.introContext.statPoint) + std::string("p)")) | bold,
                    separator(),
                    text("STR: " + strBar + " (" + std::to_string(scene.introContext.statPoints[0]) + ")") | (selectedIndex == 0 ? bold : nothing),
                    text("DEX: " + dexBar + " (" + std::to_string(scene.introContext.statPoints[1]) + ")") | (selectedIndex == 1 ? bold : nothing),
                    text("INT: " + intBar + " (" + std::to_string(scene.introContext.statPoints[2]) + ")") | (selectedIndex == 2 ? bold : nothing),
                    text("WIS: " + wisBar + " (" + std::to_string(scene.introContext.statPoints[3]) + ")") | (selectedIndex == 3 ? bold : nothing),
                    text("CHA: " + chaBar + " (" + std::to_string(scene.introContext.statPoints[4]) + ")") | (selectedIndex == 4 ? bold : nothing),
                    text("DEF: " + defBar + " (" + std::to_string(scene.introContext.statPoints[5]) + ")") | (selectedIndex == 5 ? bold : nothing),
                    text("SPD: " + spdBar + " (" + std::to_string(scene.introContext.statPoints[6]) + ")") | (selectedIndex == 6 ? bold : nothing)
                });
    }
}