#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/InputCustomClassState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void InputCustomClassState::OnEnter(IntroScene& scene)
    {
        typedBuffer = "";
        scene.introContext.input = ftxui::Input(&typedBuffer, "사용자 정의 클래스를 묘사해주세요...");
        scene.introContext.input->TakeFocus();
        scene.introContext.customClass = "";
        scene.introContext.customClassDesc = "";
    }
    void InputCustomClassState::OnExit(IntroScene& scene)
    {

    }
    void InputCustomClassState::Update(IntroScene& scene)
    {

    }
    void InputCustomClassState::HandleInput(IntroScene& scene, const Event& event)
    {
        //esc
        if(event == Event::Escape)
        {
            scene.ChangeState(IntroStep::CustomClass);
            return;
        }
        if(event == ftxui::Event::Return)
        {
            if(!typedBuffer.empty())
            {
                scene.introContext.customClass = typedBuffer;
                scene.ChangeState(IntroStep::ProcessingWriteClass);
                return;
            }
        }
        scene.introContext.input->OnEvent(event);
    }
    Element InputCustomClassState::Render(IntroScene& scene)
    {
        return vbox({
                hbox({
                    text("사용자 정의 클래스를 입력해주세요. 직업의 특징도 같이 입력한다면 완성도가 올라갈 수 있습니다."), text("예시) 전사: 단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자.") | italic}),
                scene.introContext.input->Render() | color(Color::Cyan) | underlined,
            });
    }
}