#pragma once
#include <nlohmann/json.hpp>
#include "Core/Scene/SceneBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <unordered_map>
#include <vector>
#include <future>
#include <chrono>
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

    class IntroScene : public SceneBase
    {
        private:
            std::string userName = "";
            bool userGender = false; // f: Male, t: Female
            std::string typedBuffer = "";
            TTOT::Core::Scenes::IntroStep introStep = TTOT::Core::Scenes::IntroStep::InputName;
            ftxui::Component input;
            std::unordered_map<std::string, int> savedName;
            TTOT::Core::Scenes::IntroStep prevStep;
            int spinnerIndex = 0;
            std::vector<std::string> spinner = {"🕐", "🕑", "🕒", "🕓", "🕔", "🕕", "🕖", "🕗", "🕘", "🕙", "🕚", "🕛"};
            std::chrono::steady_clock::time_point lastUpdateTime;
            std::vector<std::string> basicClass = {"전사", "도적", "궁수", "마법사", "성직자"};
            std::vector<std::string> basicClassDesc = {
                "단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자.",
                "어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자.",
                "거리를 유지한 채 치명적인 화살로 적을 숨막히게 하는 저격수.",
                "세계의 원소를 다루어 광역 파괴를 불러오는 유리 대포.",
                "신성한 힘으로 아군을 치유하고 불사의 존재를 정화하는 인도자."
            };
            std::string customClass = "";
            std::string customClassDesc = "";
            std::unique_ptr<TTOT::Class::ClassBase> classInfo;
            std::string className = "";
            std::string classDesc = "";
            std::string adventurePurpose = "";
            std::string guidance;
            std::future<std::string> geminiFuture;
            double elapsedTime;
            uint32_t statPoint = 10;
            std::vector<uint32_t> statPoints;

        public:
            IntroScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}