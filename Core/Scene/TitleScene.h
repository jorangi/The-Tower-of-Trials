#pragma once
#include "Core/Scene/IScene.h"
#include "Core/GameContext.h"

namespace TTOT::Core::Scenes
{
    class TitleScene : public IScene
    {
        private:
            TTOT::Core::GameContext& context;
        public:
            TitleScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const std::string& input) override;
            void InputError(const std::string& errorMessage) override;
    };
}