#include "Core/Scene/IntroSceneState/FinalConfirmState.h"
#include "Class/ClassBase.h"
#include "Core/Event/AddStatModifiableEvent.h"
#include "Core/Event/ChangeHealthEvent.h"
#include "Core/Event/ChangeManaEvent.h"
#include "Core/Event/CreatingEntityEvent.h"
#include "Core/Scene/IntroScene.h"
#include "Datas/EntityDTO.h"
#include "Stat/StatModifier.h"
#include "ftxui/screen/color.hpp"
#include <cmath>

#undef RGB

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;
using namespace TTOT::Class;

namespace TTOT::Core::Scenes::IntroSceneState {
void FinalConfirmState::OnEnter(IntroScene &scene) {
  selectedIndex = 0;

  const auto &classInfo = scene.introContext.classInfo;
  classHP = (int)classInfo->GetHp();
  classMP = (int)classInfo->GetMp();
  classSTR = (int)classInfo->GetStr();
  classDEX = (int)classInfo->GetDex();
  classINT = (int)classInfo->GetInt();
  classWIS = (int)classInfo->GetWis();
  classCHA = (int)classInfo->GetCha();
  classDEF = (int)classInfo->GetDef();
  classSPD = (int)classInfo->GetSpd();

  classHPMul = classInfo->GetHpMultiplier() * 100;
  classMPMul = classInfo->GetMpMultiplier() * 100;
  classStrMul = classInfo->GetStrMultiplier() * 100;
  classDexMul = classInfo->GetDexMultiplier() * 100;
  classIntMul = classInfo->GetIntMultiplier() * 100;
  classWisMul = classInfo->GetWisMultiplier() * 100;
  classChaMul = classInfo->GetChaMultiplier() * 100;
  classDefMul = classInfo->GetDefMultiplier() * 100;
  classSpdMul = classInfo->GetSpdMultiplier() * 100;
}
void FinalConfirmState::OnExit(IntroScene &scene) {}
void FinalConfirmState::Update(IntroScene &scene) {}
void FinalConfirmState::HandleInput(IntroScene &scene, const Event &event) {
  // esc
  if (event == ftxui::Event::Escape) {
    scene.ChangeState(IntroStep::amendData);
    return;
  }
  // 좌우
  if (event == ftxui::Event::ArrowLeft ||
      event == ftxui::Event::Character('a') ||
      event == ftxui::Event::ArrowRight ||
      event == ftxui::Event::Character('d')) {
    selectedIndex = (selectedIndex + 1) % 2;
  }
  // Enter
  if (event == ftxui::Event::Return) {
    if (selectedIndex == 0) {
      std::ostringstream oss;
      const auto &statPoints = scene.introContext.statPoints;

      oss << "HP: " << classHP << " + 10" << (classHPMul >= 100 ? "(↑" : "(↓")
          << std::abs(classHPMul) << "%)\n";
      oss << "MP: " << classMP << " + 10" << (classMPMul >= 100 ? "(↑" : "(↓")
          << std::abs(classMPMul) << "%)\n";
      oss << "STR: " << classSTR << " + " << statPoints[0]
          << (classStrMul >= 100 ? "(↑" : "(↓") << std::abs(classStrMul)
          << "%)\n";
      oss << "DEX: " << classDEX << " + " << statPoints[1]
          << (classDexMul >= 100 ? "(↑" : "(↓") << std::abs(classDexMul)
          << "%)\n";
      oss << "INT: " << classINT << " + " << statPoints[2]
          << (classIntMul >= 100 ? "(↑" : "(↓") << std::abs(classIntMul)
          << "%)\n";
      oss << "WIS: " << classWIS << " + " << statPoints[3]
          << (classWisMul >= 100 ? "(↑" : "(↓") << std::abs(classWisMul)
          << "%)\n";
      oss << "CHA: " << classCHA << " + " << statPoints[4]
          << (classChaMul >= 100 ? "(↑" : "(↓") << std::abs(classChaMul)
          << "%)\n";
      oss << "DEF: " << classDEF << " + " << statPoints[5]
          << (classDefMul >= 100 ? "(↑" : "(↓") << std::abs(classDefMul)
          << "%)\n";
      oss << "SPD: " << classSPD << " + " << statPoints[6]
          << (classSpdMul >= 100 ? "(↑" : "(↓") << std::abs(classSpdMul)
          << "%)\n";
      scene.introContext.tempStat = oss.str();
      TTOT::Datas::EntityDTOBuilder builder;
      builder.Name(scene.introContext.userName)
          .Hp(10)
          .Mp(10)
          .Str(scene.introContext.statPoints[0])
          .Dex(scene.introContext.statPoints[1])
          .Int(scene.introContext.statPoints[2])
          .Wis(scene.introContext.statPoints[3])
          .Cha(scene.introContext.statPoints[4])
          .Def(scene.introContext.statPoints[5])
          .Spd(scene.introContext.statPoints[6])
          .Money(0)
          .Skills({})
          .ClassInfo(std::move(scene.introContext.classInfo));

      auto playerDTO = builder.Build();
      uint32_t playerId;
      scene.context.eventBus.Publish(TTOT::Core::Events::CreatingEntityEvent{
          &playerId, scene.introContext.userName, scene.introContext.userGender,
          TTOT::Core::Events::EntityType::Player, std::move(playerDTO)});
      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::HP,
          TTOT::Stats::StatModifier(classHP, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::MP,
          TTOT::Stats::StatModifier(classMP, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Str,
          TTOT::Stats::StatModifier(classSTR, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Dex,
          TTOT::Stats::StatModifier(classDEX, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Int,
          TTOT::Stats::StatModifier(classINT, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Wis,
          TTOT::Stats::StatModifier(classWIS, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Cha,
          TTOT::Stats::StatModifier(classCHA, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Def,
          TTOT::Stats::StatModifier(classDEF, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(TTOT::Core::Events::AddStatModifiableEvent{
          playerId, TTOT::Core::Events::StatType::Spd,
          TTOT::Stats::StatModifier(classSPD, 10000,
                                    Stats::StatModifierType::Flat)});

      scene.context.eventBus.Publish(
          TTOT::Core::Events::ChangeHealthEvent{playerId, classHP});
      scene.context.eventBus.Publish(
          TTOT::Core::Events::ChangeManaEvent{playerId, classMP});

      scene.ChangeState(IntroStep::Purpose);
    } else {
      scene.ChangeState(IntroStep::amendData);
    }
  }
}
Element FinalConfirmState::Render(IntroScene &scene) {
  return vbox(
      {text("이름: " + scene.introContext.userName),
       text("성별: " +
            std::string(scene.introContext.userGender ? "여자" : "남자")),
       text("클래스: " + scene.introContext.customClass), text("[스탯]") | bold,
       hbox({text(std::string("HP: ")), text(std::to_string(classHP)),
             text(std::string(" + 10")),
             text(std::string((classHPMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classHPMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::RGB(255, 75, 75)),
       hbox({text(std::string("MP: ")), text(std::to_string(classMP)),
             text(std::string(" + 10")),
             text(std::string((classMPMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classMPMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::RGB(0, 191, 255)),
       hbox({text(std::string("STR: ")), text(std::to_string(classSTR)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[0])),
             text(std::string((classStrMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classStrMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::RGB(255, 99, 71)),
       hbox({text(std::string("DEX: ")), text(std::to_string(classDEX)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[1])),
             text(std::string((classDexMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classDexMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::GreenLight),
       hbox({text(std::string("INT: ")), text(std::to_string(classINT)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[2])),
             text(std::string((classIntMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classIntMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::CyanLight),
       hbox({text(std::string("WIS: ")), text(std::to_string(classWIS)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[3])),
             text(std::string((classWisMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classWisMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::RGB(186, 85, 211)),
       hbox({text(std::string("CHA: ")), text(std::to_string(classCHA)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[4])),
             text(std::string((classChaMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classChaMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::RGB(255, 215, 0)),
       hbox({text(std::string("DEF: ")), text(std::to_string(classDEF)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[5])),
             text(std::string((classDefMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classDefMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::GrayLight),
       hbox({text(std::string("SPD: ")), text(std::to_string(classSPD)),
             text(std::string(" + ")),
             text(std::to_string(scene.introContext.statPoints[6])),
             text(std::string((classSpdMul >= 100 ? "(↑" : "(↓")) +
                  std::to_string(abs(static_cast<int>(classSpdMul))) +
                  std::string("%)")) |
                 color(Color::DarkTurquoise)}) |
           color(Color::Yellow),
       separator(), text("이 정보가 맞습니까?"),
       hbox({text("네") | (selectedIndex == 0 ? bold : nothing), text(" / "),
             text("아니오") | (selectedIndex == 1 ? bold : nothing)})});
}
} // namespace TTOT::Core::Scenes::IntroSceneState