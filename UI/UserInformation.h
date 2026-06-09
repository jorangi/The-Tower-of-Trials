#pragma once
#include "UI/IUI.h"
#include "Entity/Player.h"

namespace TTOT::UI
{
    class UserInformation : public IUI
    {
        private:
            const TTOT::Entities::Player& states;
        public:
            UserInformation(const TTOT::Entities::Player& currStates);
            ftxui::Element Render() override;
    };
}