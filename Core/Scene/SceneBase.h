#pragma once
#include "Core/Scene/IScene.h"
#include "Core/GameContext.h"

namespace TTOT::Core::Scenes
{
    class SceneBase : public IScene
    {
        protected:
            TTOT::Core::GameContext& context;
            std::string outputText;
            int selectedIndex = 0;
        public:
            SceneBase(TTOT::Core::GameContext& context) : context(context), outputText("") {}
            virtual ~SceneBase() = default;
            void OnEnter() override {};
            void OnExit() override {};
            void Update() override {};
            void HandleInput(const ftxui::Event& event) override {};
            void InputError(const std::string& errorMessage) override 
            {
                outputText = "오류 : " + errorMessage;
            }
            ftxui::Element Render() override {return ftxui::text("");};
    };
}