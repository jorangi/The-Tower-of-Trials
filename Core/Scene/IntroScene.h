#pragma once
#include <nlohmann/json.hpp>
#include "Core/Scene/IntroSceneState/FinalConfirmState.h"
#include "Core/Scene/SceneBase.h"
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"
#include "Core/Scene/IntroSceneState/InputNameState.h"
#include "Core/Scene/IntroSceneState/ProcessingWriteClassState.h"
#include "Core/Scene/IntroSceneState/FinalConfirmState.h"
#include "Core/Scene/IntroSceneState/PurposeState.h"
#include "Core/Scene/IntroSceneState/ProcessingWritePurposeState.h"
#include "IntroSceneState/ProposalState.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <unordered_map>
#include <vector>
#include <future>
#include <Class/ClassBase.h>

namespace TTOT::Core::Scenes
{
    enum class IntroStep
    {
        InputName,
        ConfirmName,
        SelectGender,
        personalDataConfirm,
        CustomClass,
        SelectClass,
        InputCustomClass,
        ProcessingWriteClass,
        ConfirmClass,
        StatPoint,
        ConfirmStats,
        finalConfirm,
        amendData,
        Purpose,
        ProcessingWritePurpose,
        Proposal,
    };
    struct IntroContext
    {
        std::string userName;
        std::unordered_map<std::string, int> savedName;
        TTOT::Core::Scenes::IntroStep prevStep;
        bool userGender = false; // false: Male, true: Female
        std::future<std::string> geminiFuture;
        std::vector<std::string> spinner = {"🕐", "🕑", "🕒", "🕓", "🕔", "🕕", "🕖", "🕗", "🕘", "🕙", "🕚", "🕛"};
        std::vector<std::string> basicClass = {"전사", "도적", "궁수", "마법사", "성직자"};
        std::vector<std::string> basicClassDesc = {
            "단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자.",
            "어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자.",
            "거리를 유지한 채 치명적인 화살로 적을 숨막히게 하는 저격수.",
            "세계의 원소를 다루어 광역 파괴를 불러오는 유리 대포.",
            "신성한 힘으로 아군을 치유하고 불사의 존재를 정화하는 인도자."
        };
        std::unique_ptr<TTOT::Class::ClassBase> classInfo;
        std::string customClass;
        std::string customClassDesc;
        uint32_t statPoint = 10;
        std::vector<uint32_t> statPoints;
        std::string adventurePurpose = "";
        ftxui::Component input;
        std::string guidance;
        std::string tempStat;
    };
    class IntroScene : public SceneBase
    {
        friend class IntroSceneState::InputNameState;
        friend class IntroSceneState::ProcessingWriteClassState;
        friend class IntroSceneState::FinalConfirmState;
        friend class IntroSceneState::PurposeState;
        friend class IntroSceneState::ProcessingWritePurposeState;
        friend class IntroSceneState::ProposalState;
        
        private:
            std::unordered_map<TTOT::Core::Scenes::IntroStep, std::unique_ptr<TTOT::Core::Scenes::IntroSceneState::IIntroSceneState>> States;
            TTOT::Core::Scenes::IntroSceneState::IIntroSceneState* activeState = nullptr;
            TTOT::Core::Scenes::IntroStep introStep = TTOT::Core::Scenes::IntroStep::InputName;

        public:
            IntroContext introContext;
            IntroScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
            int& GetSelectedIndex(){return selectedIndex;}
            void SetSelectedIndex(int index){selectedIndex = index;}
            void ChangeState(TTOT::Core::Scenes::IntroStep newState);
    };
}