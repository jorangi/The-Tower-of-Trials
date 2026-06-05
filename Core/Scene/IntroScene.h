#pragma once
#define NOMINMAX
#include "Core/Scene/SceneBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <unordered_map>

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
        finalConfirm
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

        public:
            IntroScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}