#pragma once
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"

namespace TTOT::Core::Scenes::IntroSceneState
{
    class PurposeState : public IIntroSceneState
    {
        private:
            std::string typedBuffer = "";
        public:
            virtual void OnEnter(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void OnExit(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void Update(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void HandleInput(TTOT::Core::Scenes::IntroScene& scene, const ftxui::Event& event) override;
            virtual ftxui::Element Render(TTOT::Core::Scenes::IntroScene& scene) override;
    };
}