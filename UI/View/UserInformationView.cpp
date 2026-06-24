#include "UI/View/UserInformationView.h"
#include "UserInformationView.h"

using namespace TTOT::UI::View;
using namespace ftxui;

namespace TTOT::UI::View {
Element UserInformationView::Render() {
  return vbox({window(
      text((gender ? "🧑🏻" : "👩🏻") + std::string("  플레이어정보")),
      vbox({
          hbox({text("이름: "), text(name)}) | color(Color::Cyan) | bold,
          separator(),
          hbox({text("클래스: "), text(className)}) | color(Color::Orange1) |
              bold,
          hbox({text("소지금 : " + std::to_string(money) + "G")}) |
              color(Color::Gold1) | bold,
          separator(),
          hbox({text("HP: " + std::to_string(currentHP) + " / " +
                     std::to_string(maxHP)) |
                color(Color::Red) | bold}),
          hbox({text("MP: " + std::to_string(currentMP) + " / " +
                     std::to_string(maxMP)) |
                color(Color::Aquamarine1) | bold}),
          hbox({text("STR: "), text(std::to_string(str))}) |
              color(Color::Green1) | bold,
          hbox({text("DEX: "), text(std::to_string(dex))}) |
              color(Color::Green1) | bold,
          hbox({text("INT: "), text(std::to_string(_int))}) |
              color(Color::Green1) | bold,
          hbox({text("WIS: "), text(std::to_string(wis))}) |
              color(Color::Green1) | bold,
          hbox({text("CHA: "), text(std::to_string(cha))}) |
              color(Color::Green1) | bold,
          hbox({text("DEF: "), text(std::to_string(def))}) |
              color(Color::Green1) | bold,
          hbox({text("SPD: "), text(std::to_string(spd))}) |
              color(Color::Green1) | bold,
      }))});
}
} // namespace TTOT::UI::View