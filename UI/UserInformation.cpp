#include "UI/UserInformation.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Component/ClassComponent.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

using namespace TTOT::UI;
using namespace ftxui;

namespace TTOT::UI
{
    UserInformation::UserInformation(const TTOT::Entities::Player& currStates) : states(currStates) {}
    Element UserInformation::Render()
    {
        auto hpcomp = states.GetComponent<TTOT::Components::HealthComponent>();
        auto mpcomp = states.GetComponent<TTOT::Components::ManaComponent>();
        auto moneycomp = states.GetComponent<TTOT::Components::MoneyComponent>();
        auto statscomp = states.GetComponent<TTOT::Components::StatComponent>();
        auto classcomp = states.GetComponent<TTOT::Components::ClassComponent>();
        return vbox({
            window(
                text((states.GetGender() ? "👩🏻" : "🧑🏻") + std::string("  플레이어정보")), 
                vbox({
                    hbox({text("이름: "), text(states.GetName())})  | color(Color::Cyan) | bold,
                    separator(),
                    hbox({text("클래스: "), text(classcomp->GetClassInfo()->GetName())}) | color(Color::Orange1) | bold,
                    hbox({text(moneycomp->ToString())}) | color(Color::Gold1) | bold,
                    separator(),
                    hbox({text(hpcomp->ToString()) | color(Color::Red) | bold}),
                    hbox({text(mpcomp->ToString()) | color(Color::Aquamarine1) | bold}),
                    hbox({text("STR: "), text(std::to_string(statscomp->GetSTR()))}) | color(Color::Green1) | bold,
                    hbox({text("DEX: "), text(std::to_string(statscomp->GetDEX()))}) | color(Color::Green1) | bold,
                    hbox({text("INT: "), text(std::to_string(statscomp->GetINT()))}) | color(Color::Green1) | bold,
                    hbox({text("WIS: "), text(std::to_string(statscomp->GetWIS()))}) | color(Color::Green1) | bold,
                    hbox({text("CHA: "), text(std::to_string(statscomp->GetCHA()))}) | color(Color::Green1) | bold,
                    hbox({text("DEF: "), text(std::to_string(statscomp->GetDEF()))}) | color(Color::Green1) | bold,
                    hbox({text("SPD: "), text(std::to_string(statscomp->GetSPD()))}) | color(Color::Green1) | bold,
                }))
        });
    }
}