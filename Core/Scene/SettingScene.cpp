#include "Core/Scene/SettingScene.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/VolumeChangeEvent.h"
#include "Core/Event/VolumeGetEvent.h"
#include <cmath>
#include <algorithm>

namespace TTOT::Core::Scenes
{
    SettingScene::SettingScene(TTOT::Core::GameContext& context) : SceneBase(context) {}
    void SettingScene::OnEnter()
    {

        context.eventBus.Publish(TTOT::Core::Events::VolumeGetEvent{
            [this](float master, float bgm, float sfx)
            {
                this->masterVolume = static_cast<int>(std::round(master * 100.0f));
                this->bgmVolume = static_cast<int>(std::round(bgm * 100.0f));
                this->sfxVolume = static_cast<int>(std::round(sfx * 100.0f));
            }
        });
    }
    void SettingScene::OnExit(){}
    void SettingScene::Update(){}
    void SettingScene::HandleInput(const ftxui::Event& event)
    {
        if(event == ftxui::Event::Escape)
        {
            context.eventBus.Publish(TTOT::Core::Events::SceneChangeEvent{context.settingReturnSceneId});
            return;
        }
        
        if(event == ftxui::Event::Character('w') || event == ftxui::Event::ArrowUp)
        {
            selectedIndex = (selectedIndex - 1 + 3) % 3;
        }
        else if(event == ftxui::Event::Character('s') || event == ftxui::Event::ArrowDown)
        {
            selectedIndex = (selectedIndex + 1) % 3;
        }
        else if(event == ftxui::Event::Character('a') || event == ftxui::Event::ArrowLeft)
        {
            switch(selectedIndex)
            {
                case 0:
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::Master, (std::max)(0, masterVolume - 10)});
                    break;
                case 1:
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::BGM, (std::max)(0, bgmVolume - 10)});
                    break;
                case 2:
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::SFX, (std::max)(0, sfxVolume - 10)});
                    break;
            }
            context.eventBus.Publish(TTOT::Core::Events::VolumeGetEvent{
                [this](float master, float bgm, float sfx)
                {
                    this->masterVolume = static_cast<int>(std::round(master * 100.0f));
                    this->bgmVolume = static_cast<int>(std::round(bgm * 100.0f));
                    this->sfxVolume = static_cast<int>(std::round(sfx * 100.0f));
                }
            });
        }
        else if(event == ftxui::Event::Character('d') || event == ftxui::Event::ArrowRight)
        {
            switch(selectedIndex)
            {
                case 0: 
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::Master, (std::min)(100, masterVolume + 10)});
                    break;
                case 1: 
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::BGM, (std::min)(100, bgmVolume + 10)});
                    break;
                case 2: 
                    context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::SFX, (std::min)(100, sfxVolume + 10)});
                    break;
            }
            context.eventBus.Publish(TTOT::Core::Events::VolumeGetEvent{
            [this](float master, float bgm, float sfx)
                {
                    this->masterVolume = static_cast<int>(std::round(master * 100.0f));
                    this->bgmVolume = static_cast<int>(std::round(bgm * 100.0f));
                    this->sfxVolume = static_cast<int>(std::round(sfx * 100.0f));
                }
            });
        }
        else
        {
            InputError("잘못된 입력입니다.");
        }
    }
    ftxui::Element TTOT::Core::Scenes::SettingScene::Render()
    {
        using namespace ftxui;
        std::string masterBar;
        std::string bgmBar;
        std::string sfxBar;
        for(int i = 0; i < 10; i++)
        {
            if(i < masterVolume / 10) masterBar += "■";
            else masterBar += " ";
            if(i < bgmVolume / 10) bgmBar += "■";
            else bgmBar += " ";
            if(i < sfxVolume / 10) sfxBar += "■";
            else sfxBar += " ";
        }
        return vbox({
            hbox({text("설정 ⚙")}),
            hbox({
                text("마스터 볼륨: \t\t") | (selectedIndex == 0 ? bold : nothing), text(masterBar + " (" + std::to_string(masterVolume) + "%)")
            }),
            hbox({
                text("BGM: \t\t") | (selectedIndex == 1 ? bold : nothing), text(bgmBar + " (" + std::to_string(bgmVolume) + "%)")
            }),
            hbox({
                text("SFX: \t\t") | (selectedIndex == 2 ? bold : nothing), text(sfxBar + " (" + std::to_string(sfxVolume) + "%)")
            }),
        }) | border;
    }
}