#pragma once
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"
#include <chrono>

namespace TTOT::Core::Scenes::IntroSceneState
{
    class ProcessingWriteClassState : public IIntroSceneState
    {
        private:
            int spinnerIndex = 0;
            std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;
        public:
            virtual void OnEnter(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void OnExit(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void Update(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void HandleInput(TTOT::Core::Scenes::IntroScene& scene, const ftxui::Event& event) override;
            virtual ftxui::Element Render(TTOT::Core::Scenes::IntroScene& scene) override;
    };
}