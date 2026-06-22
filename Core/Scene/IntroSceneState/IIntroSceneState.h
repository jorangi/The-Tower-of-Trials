#pragma once
#include <ftxui/component/event.hpp>
#include <ftxui/component/component.hpp>

namespace TTOT::Core::Scenes { 
    class IntroScene; 
}
namespace TTOT::Core::Scenes::IntroSceneState
{
    class IIntroSceneState
    {
        public:
            virtual ~IIntroSceneState() = default;
            virtual void OnEnter(TTOT::Core::Scenes::IntroScene& scene) = 0;
            virtual void OnExit(TTOT::Core::Scenes::IntroScene& scene) = 0;
            virtual void Update(TTOT::Core::Scenes::IntroScene& scene) = 0;
            virtual void HandleInput(TTOT::Core::Scenes::IntroScene& scene, const ftxui::Event& event) = 0;
            virtual ftxui::Element Render(TTOT::Core::Scenes::IntroScene& scene) = 0;
    };
}