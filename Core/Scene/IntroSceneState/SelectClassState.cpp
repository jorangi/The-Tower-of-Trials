#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/SelectClassState.h"
#include "Class/BasicClassTemplates.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void SelectClassState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void SelectClassState::OnExit(IntroScene& scene)
    {

    }
    void SelectClassState::Update(IntroScene& scene)
    {

    }
    void SelectClassState::HandleInput(IntroScene& scene, const Event& event)
    {
        //Esc
        if(event == Event::Escape)
        {
            scene.introContext.customClass = "";
            scene.introContext.customClassDesc = "";
            scene.ChangeState(IntroStep::CustomClass);
            return;
        }
        //상하
        if(event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('w'))    
            selectedIndex = (selectedIndex - 1 + 5) % 5;
        if(event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('s'))  
            selectedIndex = (selectedIndex + 1) % 5;
        //Enter
        if(event == Event::Return)
        {
            const auto& parsedJson = classTemplates[selectedIndex];
            scene.introContext.classInfo = std::make_unique<TTOT::Class::ClassBase>(parsedJson.get<TTOT::Class::ClassBase>());
            scene.introContext.customClass = scene.introContext.classInfo->GetName();
            scene.introContext.customClassDesc = scene.introContext.classInfo->GetDesc();
            selectedIndex = 0;
            scene.ChangeState(IntroStep::StatPoint);
            return;
        }
    }
    Element SelectClassState::Render(IntroScene& scene)
    {
        const auto& classTemplates = TTOT::Class::BASIC_CLASS_TEMPLATES;
        return vbox({
            text("클래스를 선택해주세요."),
                vbox({
                    text(classTemplates[0]["className"].get<std::string>() + ": " + classTemplates[0]["classDesc"].get<std::string>()) 
                        | color(Color::Red) | (selectedIndex == 0 ? bold : nothing),

                    text(classTemplates[1]["className"].get<std::string>() + ": " + classTemplates[1]["classDesc"].get<std::string>()) 
                        | color(Color::NavyBlue) | (selectedIndex == 1 ? bold : nothing),

                    text(classTemplates[2]["className"].get<std::string>() + ": " + classTemplates[2]["classDesc"].get<std::string>()) 
                        | color(Color::Green) | (selectedIndex == 2 ? bold : nothing),

                    text(classTemplates[3]["className"].get<std::string>() + ": " + classTemplates[3]["classDesc"].get<std::string>()) 
                        | color(Color::Purple) | (selectedIndex == 3 ? bold : nothing),

                    text(classTemplates[4]["className"].get<std::string>() + ": " + classTemplates[4]["classDesc"].get<std::string>()) 
                        | color(Color::Gold1) | (selectedIndex == 4 ? bold : nothing)
                })
        });
    }
}