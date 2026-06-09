#include "Core/Event/GameLoopEvent.h"
#include "Core/Event/VolumeChangeEvent.h"
#include "Core/Event/VolumeGetEvent.h"
#include "Core/GameManager.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "Core/Scene/TitleScene.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/SoundEvent.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Core/Scene/SettingScene.h"
#include "Core/Scene/IntroScene.h"
#include "Core/Scene/FieldScene.h"
#include "Event/CreatingEntityEvent.h"
#include "Event/PlayerGetEvent.h"

namespace TTOT::Core
{
    GameManager::GameManager() : isRunning(false), gameState(TTOT::Core::GameState::None), screen(ftxui::ScreenInteractive::Fullscreen())
    {
        gemini = std::make_unique<TTOT::Network::ConnGemini>();
        sceneManager = std::make_unique<TTOT::Core::SceneManager>();
        soundManager = std::make_unique<TTOT::Utilities::SoundManager>();
        imageRenderer = std::make_unique<TTOT::Utilities::ImageRenderer>();

        eventBus.Subscribe<TTOT::Core::Events::SoundEvent>([this](const TTOT::Core::Events::SoundEvent& event)
        {
            if(event.key.rfind("BGM_", 0) == 0)
            {
                soundManager->PlayBGM(event.key);
            }
            else
            {
                soundManager->PlaySFX(event.key);
            }
        });
        eventBus.Subscribe<TTOT::Core::Events::GameLoopEvent>([this](const TTOT::Core::Events::GameLoopEvent& event)
        {
            if(event.action == TTOT::Core::Events::GameLoopAction::Exitgame)
            {
                isRunning = false;
                screen.Exit();
            }
        });
        eventBus.Subscribe<TTOT::Core::Events::SceneChangeEvent>([this](const TTOT::Core::Events::SceneChangeEvent& event)
        {
            this->nextSceneId = event.sceneId;
            this->screen.PostEvent(ftxui::Event::Custom);
        });
        eventBus.Subscribe<TTOT::Core::Events::ScreenRefreshEvent>([this](const TTOT::Core::Events::ScreenRefreshEvent& event)
        {
            this->screen.PostEvent(ftxui::Event::Custom);
        });
        eventBus.Subscribe<TTOT::Core::Events::VolumeChangeEvent>([this](const TTOT::Core::Events::VolumeChangeEvent& event)
        {
            switch(event.type)
            {
                case TTOT::Core::Events::VolumeType::Master:
                    soundManager->SetMasterVolume(static_cast<float>(event.value) * 0.01f);
                    break;
                case TTOT::Core::Events::VolumeType::BGM:
                    soundManager->SetBGMVolume(static_cast<float>(event.value) * 0.01f);
                    break;
                case TTOT::Core::Events::VolumeType::SFX:
                    soundManager->SetSFXVolume(static_cast<float>(event.value) * 0.01f);
                    break;
            }
        });
        eventBus.Subscribe<TTOT::Core::Events::VolumeGetEvent>([this](const TTOT::Core::Events::VolumeGetEvent& event)
        {
            auto volumes = soundManager->GetVolumes();
            event.onLoaded(volumes.masterVolume, volumes.bgmVolume, volumes.sfxVolume);
        });
        eventBus.Subscribe<TTOT::Core::Events::CreatingEntityEvent>([this](const TTOT::Core::Events::CreatingEntityEvent& event)
        {
            if(event.entityType == TTOT::Core::Events::EntityType::Player)
            {
                this->player = this->playerFactory->CreatePlayer(std::move(event.dto), false);
            }
            else if(event.entityType == TTOT::Core::Events::EntityType::Monster)
            {
                auto m = this->monsterFactory->CreateMonster(event.dto);
            }
        });
        eventBus.Subscribe<TTOT::Core::Events::PlayerGetEvent>([this](const TTOT::Core::Events::PlayerGetEvent& event)
        {
            if(this->player != nullptr) event.onLoaded(*(this->player));
        });
    }
    void GameManager::Run()
    {
        isRunning = true;
        gameState = TTOT::Core::GameState::Title;
        sceneManager->RegisterScene<TTOT::Core::Scenes::TitleScene>(0);
        sceneManager->RegisterScene<TTOT::Core::Scenes::SettingScene>(1);
        sceneManager->RegisterScene<TTOT::Core::Scenes::IntroScene>(2);
        sceneManager->RegisterScene<TTOT::Core::Scenes::FieldScene>(3);
        GameContext context{eventBus, *imageRenderer, *gemini};
        this->currentScene = sceneManager->LoadScene(0, context);
        context.eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{TTOT::Core::Events::VolumeType::Master, 0});
        auto input_handler = ftxui::CatchEvent(ftxui::Make<ftxui::ComponentBase>(), [this](const ftxui::Event& event)
        {
            if(this->sceneManager->GetCurrentScene() != nullptr)
            {
                this->currentScene->HandleInput(event);
                return true;
            }
            return false;
        });
        auto renderer = ftxui::Renderer(input_handler, [this, &context]()
        {
            if(this->nextSceneId != -1)
            {
                if(this->currentScene != nullptr)
                {
                    this->currentScene->OnExit();
                }

                this->currentScene = sceneManager->LoadScene(this->nextSceneId, context);
                this->nextSceneId = -1;

                if(this->currentScene != nullptr)
                {
                    this->currentScene->OnEnter();
                }
            }
            gameState = TTOT::Core::GameState::GameLoop;
            if(this->currentScene != nullptr)
            {
                this->currentScene->Update();
                return this->currentScene->Render();
            }
            return ftxui::text("씬을 불러오는데 실패했습니다") | ftxui::border;
        });

        screen.Loop(renderer);
    }
}