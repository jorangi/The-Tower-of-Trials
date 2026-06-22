#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/ConfirmClassState.h"
#include "Core/Scene/IntroSceneState/ConfirmNameState.h"
#include "Core/Scene/IntroSceneState/SelectGenderState.h"
#include "Core/Scene/IntroSceneState/PersonalDataConfirmState.h"
#include "Core/Scene/IntroSceneState/CustomClassState.h"
#include "Core/Scene/IntroSceneState/SelectClassState.h"
#include "Core/Scene/IntroSceneState/InputCustomClassState.h"
#include "Core/Scene/IntroSceneState/StatPointState.h"
#include "Core/Scene/IntroSceneState/ConfirmStatsState.h"
#include "Core/Scene/IntroSceneState/AmendDataState.h"
#include <string>

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;
namespace TTOT::Core::Scenes
{
    IntroScene::IntroScene(TTOT::Core::GameContext& context) : SceneBase(context)
    {
        States[IntroStep::InputName] = std::make_unique<InputNameState>();
        States[IntroStep::ConfirmName] = std::make_unique<ConfirmNameState>();
        States[IntroStep::SelectGender] = std::make_unique<SelectGenderState>();
        States[IntroStep::personalDataConfirm] = std::make_unique<PersonalDataConfirmState>();
        States[IntroStep::CustomClass] = std::make_unique<CustomClassState>();
        States[IntroStep::SelectClass] = std::make_unique<SelectClassState>();
        States[IntroStep::InputCustomClass] = std::make_unique<InputCustomClassState>();
        States[IntroStep::ProcessingWriteClass] = std::make_unique<ProcessingWriteClassState>();
        States[IntroStep::ConfirmClass] = std::make_unique<ConfirmClassState>();
        States[IntroStep::StatPoint] = std::make_unique<StatPointState>();
        States[IntroStep::ConfirmStats] = std::make_unique<ConfirmStatsState>();
        States[IntroStep::finalConfirm] = std::make_unique<FinalConfirmState>();
        States[IntroStep::amendData] = std::make_unique<AmendDataState>();
        States[IntroStep::Purpose] = std::make_unique<PurposeState>();
        States[IntroStep::ProcessingWritePurpose] = std::make_unique<ProcessingWritePurposeState>();
        States[IntroStep::Proposal] = std::make_unique<ProposalState>();
    }
    void IntroScene::OnEnter()
    {
        //TODO: 인트로용 BGM 재생
        ChangeState(IntroStep::InputName);
    }
    void IntroScene::OnExit()
    {
        //TODO: 작성된 데이터를 저장하여 보관할 것
        if (activeState) activeState->OnExit(*this);
    }
    void IntroScene::Update()
    {
        if (activeState) 
        {
            activeState->Update(*this);
        }
    }
    void IntroScene::HandleInput(const ftxui::Event& event)
    {
        if (activeState) 
        {
            activeState->HandleInput(*this, event);
        }
    }
    Element IntroScene::Render()
    {
        if (!activeState) return text("로딩 중...") | border;

        Elements content;

        content.push_back(text("✨새로운 여정의 시작✨") | hcenter | bold);
        content.push_back(separator());

        content.push_back(activeState->Render(*this));
        
        return vbox(std::move(content)) | border;
    }
    void IntroScene::ChangeState(TTOT::Core::Scenes::IntroStep newState)
    {
        if(activeState) activeState->OnExit(*this);
        introContext.prevStep = introStep;
        introStep = newState;
        activeState = States[newState].get();
        if(activeState) activeState->OnEnter(*this);
    }
}