#pragma once
#include "Core/Scene/IntroSceneState/IIntroSceneState.h"
#include "Class/BasicClassTemplates.h"

namespace TTOT::Core::Scenes::IntroSceneState
{
    class SelectClassState : public IIntroSceneState
    {
        private:
            int selectedIndex = 0;
            const std::vector<nlohmann::json_abi_v3_12_0::json, std::allocator<nlohmann::json_abi_v3_12_0::json>> classTemplates = TTOT::Class::BASIC_CLASS_TEMPLATES;
        public:
            virtual void OnEnter(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void OnExit(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void Update(TTOT::Core::Scenes::IntroScene& scene) override;
            virtual void HandleInput(TTOT::Core::Scenes::IntroScene& scene, const ftxui::Event& event) override;
            virtual ftxui::Element Render(TTOT::Core::Scenes::IntroScene& scene) override;
    };
}