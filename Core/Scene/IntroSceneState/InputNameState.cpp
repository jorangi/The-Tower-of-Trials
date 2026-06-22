#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"
#include "Core/Event/SceneChangeEvent.h"

using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;
using namespace ftxui;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void InputNameState::OnEnter(IntroScene& scene)
    {
        scene.introContext.input = ftxui::Input(&typedBuffer, "이름을 입력해주세요...");
        scene.introContext.input->TakeFocus();
        scene.introContext.userName = "";
    }
    void InputNameState::OnExit(IntroScene& scene)
    {
        typedBuffer = "";
    }
    void InputNameState::Update(IntroScene& scene)
    {
    }
    void InputNameState::HandleInput(IntroScene& scene, const ftxui::Event& event)
    {
        //ESC
        if(event == ftxui::Event::Escape)
        {
            scene.context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{0});
            return;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            if(!typedBuffer.empty())
            {
                scene.introContext.userName = typedBuffer;
                if(scene.introContext.prevStep == IntroStep::ConfirmName) 
                    scene.introContext.savedName[scene.introContext.userName]++;
                scene.ChangeState(IntroStep::ConfirmName);
                typedBuffer = "";
            }
            return;
        }
        //내용 입력
        scene.introContext.input->OnEvent(event);
        

    }
    ftxui::Element InputNameState::Render(IntroScene& scene)
    {
        return hbox({text("당신의 이름은 무엇입니까?: "), scene.introContext.input->Render() | color(Color::Cyan) | underlined});
    }
}