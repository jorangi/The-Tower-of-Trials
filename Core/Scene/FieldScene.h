#include "Core/Scene/SceneBase.h"
#include "UI/UserInformation.h"
#include <memory>

namespace TTOT::Core::Scenes
{
    class FieldScene : public SceneBase
    {
        private:
            std::unique_ptr<TTOT::UI::UserInformation> userInfoUI;
        public:
            FieldScene(TTOT::Core::GameContext& context);
            void OnEnter() override;
            void OnExit() override;
            void Update() override;
            void HandleInput(const ftxui::Event& event) override;
            ftxui::Element Render() override;
    };
}