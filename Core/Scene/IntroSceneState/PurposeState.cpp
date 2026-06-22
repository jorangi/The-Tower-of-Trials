#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/PurposeState.h"

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void PurposeState::OnEnter(IntroScene& scene)
    {
        typedBuffer = "";
        scene.introContext.adventurePurpose = "";
        scene.introContext.input = ftxui::Input(&typedBuffer, "당신이 탑에 오르는 이유는...");
        scene.introContext.input->TakeFocus();
    }
    void PurposeState::OnExit(IntroScene& scene)
    {

    }
    void PurposeState::Update(IntroScene& scene)
    {

    }
    void PurposeState::HandleInput(IntroScene& scene, const ftxui::Event& event)
    {
        //Enter
        if(event == ftxui::Event::Return)
        {
            if(!typedBuffer.empty())
            {
                scene.introContext.adventurePurpose = typedBuffer;
                scene.ChangeState(IntroStep::ProcessingWritePurpose);
                return;
            }
        }
        scene.introContext.input->OnEvent(event);
    }
    Element PurposeState::Render(IntroScene& scene)
    {
        return vbox({
                    scene.context.imageRenderer.ImageRender("Assets/Images/IntroImage.png"),
                    text(""),
                    text(""),
                    paragraph("이곳은 세상 모든 이들의 갈망을 먹고 자라나는 거대한 탑.\n진정으로 무언가를 원하는 자에게만 탑은 문을 열어줍니다."),
                    paragraph("당신이 탑을 찾아온 이유는 무엇인가요?") | bold,
                    separator(),
                    scene.introContext.input->Render() | color(Color::Cyan) | underlined,
                });
    }
}