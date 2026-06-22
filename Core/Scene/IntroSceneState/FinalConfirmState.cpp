#include "Core/Scene/IntroScene.h"
#include "Core/Scene/IntroSceneState/FinalConfirmState.h"
#include "Class/ClassBase.h"
#include "ftxui/screen/color.hpp"
#include "Datas/EntityDTO.h"
#include "Core/Event/CreatingEntityEvent.h"
#include <cmath>
#undef RGB

using namespace ftxui;
using namespace TTOT::Core::Scenes;
using namespace TTOT::Core::Scenes::IntroSceneState;
using namespace TTOT::Class;

namespace TTOT::Core::Scenes::IntroSceneState
{
    void FinalConfirmState::OnEnter(IntroScene& scene)
    {
        selectedIndex = 0;
    }
    void FinalConfirmState::OnExit(IntroScene& scene)
    {

    }
    void FinalConfirmState::Update(IntroScene& scene)
    {

    }
    void FinalConfirmState::HandleInput(IntroScene& scene, const Event& event)
    {
        //esc
        if(event == ftxui::Event::Escape)
        {
            scene.ChangeState(IntroStep::amendData);
            return;
        }
        //좌우
        if(event == ftxui::Event::ArrowLeft || event == ftxui::Event::Character('a') ||
            event == ftxui::Event::ArrowRight || event == ftxui::Event::Character('d'))
        {
            selectedIndex = (selectedIndex + 1) % 2;
        }
        //Enter
        if(event == ftxui::Event::Return)
        {
            if(selectedIndex == 0)
            {
                std::ostringstream oss;
                const auto& classInfo = scene.introContext.classInfo;
                const auto& statPoints = scene.introContext.statPoints;

                oss << "HP: " << static_cast<int>(classInfo->GetHp()) << " + 10"
                    << (classInfo->GetHpMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetHpMultiplier() * 100.0)) << "%)\n";
                oss << "MP: " << static_cast<int>(classInfo->GetMp()) << " + 10"
                    << (classInfo->GetMpMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetMpMultiplier() * 100.0)) << "%)\n";
                oss << "STR: " << static_cast<int>(classInfo->GetStr()) << " + " << statPoints[0]
                    << (classInfo->GetStrMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetStrMultiplier() * 100.0)) << "%)\n";
                oss << "DEX: " << static_cast<int>(classInfo->GetDex()) << " + " << statPoints[1]
                    << (classInfo->GetDexMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetDexMultiplier() * 100.0)) << "%)\n";
                oss << "INT: " << static_cast<int>(classInfo->GetInt()) << " + " << statPoints[2]
                    << (classInfo->GetIntMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetIntMultiplier() * 100.0)) << "%)\n";
                oss << "WIS: " << static_cast<int>(classInfo->GetWis()) << " + " << statPoints[3]
                    << (classInfo->GetWisMultiplier() >= 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetWisMultiplier() * 100.0)) << "%)\n";
                oss << "CHA: " << static_cast<int>(classInfo->GetCha()) << " + " << statPoints[4]
                    << (classInfo->GetChaMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetChaMultiplier() * 100.0)) << "%)\n";
                oss << "DEF: " << static_cast<int>(classInfo->GetDef()) << " + " << statPoints[5]
                    << (classInfo->GetDefMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetDefMultiplier() * 100.0)) << "%)\n";
                oss << "SPD: " << static_cast<int>(classInfo->GetSpd()) << " + " << statPoints[6]
                    << (classInfo->GetSpdMultiplier() > 1.0 ? "(↑" : "(↓")
                    << std::abs(static_cast<int>(classInfo->GetSpdMultiplier() * 100.0)) << "%)\n";
                scene.introContext.tempStat = oss.str();
                TTOT::Datas::EntityDTOBuilder builder;
                builder.Id(0)
                    .Name(scene.introContext.userName)
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
                scene.context.eventBus.Publish(TTOT::Core::Events::CreatingEntityEvent{
                    0,
                    scene.introContext.userName,
                    scene.introContext.userGender,
                    TTOT::Core::Events::EntityType::Player,
                    std::move(playerDTO)
                });
                scene.ChangeState(IntroStep::Purpose);
            }
            else
            {
                scene.ChangeState(IntroStep::amendData);
            }
        }
    }
    Element FinalConfirmState::Render(IntroScene& scene)
    {
        return vbox({
                    text("이름: " + scene.introContext.userName),
                    text("성별: " + std::string(scene.introContext.userGender ? "여자" : "남자")),
                    text("클래스: " + scene.introContext.customClass),
                    text("[스탯]") | bold,
                    hbox({
                        text(std::string("HP: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetHp()))),
                        text(std::string(" + 10")),
                        text(std::string((scene.introContext.classInfo->GetHpMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetHpMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::RGB(255, 75, 75)),
                    hbox({
                        text(std::string("MP: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetMp()))),
                        text(std::string(" + 10")),
                        text(std::string((scene.introContext.classInfo->GetMpMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetMpMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::RGB(0, 191, 255)),
                    hbox({
                        text(std::string("STR: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetStr()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[0])),
                        text(std::string((scene.introContext.classInfo->GetStrMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetStrMultiplier()*100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::RGB(255, 99, 71)),
                    hbox({
                        text(std::string("DEX: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetDex()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[1])),
                        text(std::string((scene.introContext.classInfo->GetDexMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetDexMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::GreenLight),
                    hbox({
                        text(std::string("INT: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetInt()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[2])),
                        text(std::string((scene.introContext.classInfo->GetIntMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetIntMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::CyanLight),
                    hbox({
                        text(std::string("WIS: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetWis()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[3])),
                        text(std::string((scene.introContext.classInfo->GetWisMultiplier() >= 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetWisMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::RGB(186, 85, 211)),
                    hbox({
                        text(std::string("CHA: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetCha()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[4])),
                        text(std::string((scene.introContext.classInfo->GetChaMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetChaMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::RGB(255, 215, 0)),
                    hbox({
                        text(std::string("DEF: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetDef()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[5])),
                        text(std::string((scene.introContext.classInfo->GetDefMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetDefMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::GrayLight),
                    hbox({
                        text(std::string("SPD: ")), 
                        text(std::to_string(static_cast<int>(scene.introContext.classInfo->GetSpd()))),
                        text(std::string(" + ")),
                        text(std::to_string(scene.introContext.statPoints[6])),
                        text(std::string((scene.introContext.classInfo->GetSpdMultiplier() > 1.0 ? "(↑" : "(↓")) + 
                            std::to_string(abs(static_cast<int>(scene.introContext.classInfo->GetSpdMultiplier() * 100.0))) + std::string("%)")) | color(Color::DarkTurquoise)
                    }) | color(Color::Yellow),
                    separator(),
                    text("이 정보가 맞습니까?"),
                    hbox({
                        text("네") | (selectedIndex == 0 ? bold : nothing), text(" / "), text("아니오") | (selectedIndex == 1 ? bold : nothing)
                    })
                });
    }
}