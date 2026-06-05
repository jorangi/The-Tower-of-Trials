#pragma once
#include "Core/Scene/SceneBase.h"

namespace TTOT::Core::Scenes
{
    class SettingScene : public SceneBase
    {
        private:
            int masterVolume = 10;
            int bgmVolume = 10;
            int sfxVolume = 10;
        public:
            SettingScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}