#pragma once
#include "Core/Scene/SceneBase.h"

namespace TTOT::Core::Scenes
{
    class TitleScene : public SceneBase
    {
        public:
            TitleScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}