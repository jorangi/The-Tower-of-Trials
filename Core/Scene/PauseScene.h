#pragma once
#include "Core/Scene/SceneBase.h"

namespace TTOT::Core::Scenes
{
    enum class PauseSubState {
        MainMenu,
        SlotSelection,
        OverwriteWarning
    };

    class PauseScene : public SceneBase
    {
        private:
            PauseSubState subState = PauseSubState::MainMenu;
            int targetSlot = 1;
            int warningSelectedIndex = 1;
        public:
            PauseScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}
