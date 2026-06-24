#include "Core/GameManager.h"
#include "Class/ClassBase.h"
#include "Component/ClassComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Core/Event/GameLoopEvent.h"
#include "Core/Event/LoadCompleteEvent.h"
#include "Core/Event/LoadRequestEvent.h"
#include "Core/Event/SaveRequestEvent.h"
#include "Core/Event/SceneChangeEvent.h"
#include "Core/Event/ScreenRefreshEvent.h"
#include "Core/Event/SoundEvent.h"
#include "Core/Event/VolumeChangeEvent.h"
#include "Core/Event/VolumeGetEvent.h"
#include "Core/MasterSaveData.h"
#include "Core/SaveManager.h"
#include "Core/Scene/FieldScene.h"
#include "Core/Scene/IntroScene.h"
#include "Core/Scene/SaveFileLoadScene.h"
#include "Core/Scene/SettingScene.h"
#include "Core/Scene/TitleScene.h"
#include "Event/AddStatModifiableEvent.h"
#include "Event/CreatingEntityEvent.h"
#include "Event/InjectPurposeEvent.h"
#include "Event/PlayerGetEvent.h"
#include "Core/Event/ChangeHealthEvent.h"
#include "Core/Event/ChangeManaEvent.h"
#include <chrono>
#include <ctime>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <sstream>

namespace TTOT::Core {
GameManager::GameManager()
    : isRunning(false), gameState(TTOT::Core::GameState::None),
      screen(ftxui::ScreenInteractive::Fullscreen()) {
  gemini = std::make_unique<TTOT::Network::ConnGemini>();
  sceneManager = std::make_unique<TTOT::Core::SceneManager>();
  soundManager = std::make_unique<TTOT::Utilities::SoundManager>();
  imageRenderer = std::make_unique<TTOT::Utilities::ImageRenderer>();
  saveManager = std::make_unique<TTOT::Core::SaveManager>();
  context = std::make_unique<TTOT::Core::GameContext>(eventBus, *imageRenderer,
                                                      *gemini);
  eventBus.Subscribe<TTOT::Core::Events::SoundEvent>(
      [this](const TTOT::Core::Events::SoundEvent &event) {
        if (event.key.rfind("BGM_", 0) == 0) {
          soundManager->PlayBGM(event.key);
        } else {
          soundManager->PlaySFX(event.key);
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::GameLoopEvent>(
      [this](const TTOT::Core::Events::GameLoopEvent &event) {
        if (event.action == TTOT::Core::Events::GameLoopAction::Exitgame) {
          isRunning = false;
          screen.Exit();
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::SceneChangeEvent>(
      [this](const TTOT::Core::Events::SceneChangeEvent &event) {
        this->nextSceneId = event.sceneId;
        this->screen.PostEvent(ftxui::Event::Custom);
      });
  eventBus.Subscribe<TTOT::Core::Events::ScreenRefreshEvent>(
      [this](const TTOT::Core::Events::ScreenRefreshEvent &event) {
        this->screen.PostEvent(ftxui::Event::Custom);
      });
  eventBus.Subscribe<TTOT::Core::Events::VolumeChangeEvent>(
      [this](const TTOT::Core::Events::VolumeChangeEvent &event) {
        switch (event.type) {
        case TTOT::Core::Events::VolumeType::Master:
          soundManager->SetMasterVolume(static_cast<float>(event.value) *
                                        0.01f);
          break;
        case TTOT::Core::Events::VolumeType::BGM:
          soundManager->SetBGMVolume(static_cast<float>(event.value) * 0.01f);
          break;
        case TTOT::Core::Events::VolumeType::SFX:
          soundManager->SetSFXVolume(static_cast<float>(event.value) * 0.01f);
          break;
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::VolumeGetEvent>(
      [this](const TTOT::Core::Events::VolumeGetEvent &event) {
        auto volumes = soundManager->GetVolumes();
        event.onLoaded(volumes.masterVolume, volumes.bgmVolume,
                       volumes.sfxVolume);
      });
  eventBus.Subscribe<TTOT::Core::Events::CreatingEntityEvent>(
      [this](const TTOT::Core::Events::CreatingEntityEvent &event) {
        uint32_t entityId = nextEntityId;
        if (event.entityType == TTOT::Core::Events::EntityType::Player) {
          this->player = this->playerFactory->CreatePlayer(
              entityId, std::move(event.dto), false);
          RegistryEntity(this->player.get());
        } else if (event.entityType ==
                   TTOT::Core::Events::EntityType::Monster) {
          auto m = this->monsterFactory->CreateMonster(entityId, event.dto);
          RegistryEntity(m.get());
        }
        *(event.entityId) = entityId;
      });
  eventBus.Subscribe<TTOT::Core::Events::AddStatModifiableEvent>(
      [this](const TTOT::Core::Events::AddStatModifiableEvent &event) {
        auto entity = this->GetEntity(event.entityId);
        if (entity == nullptr)
          return;
        switch (event.statType) {
        case TTOT::Core::Events::StatType::HP:
          entity->GetComponent<TTOT::Components::HealthComponent>()
              ->GetMaxHP()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::MP:
          entity->GetComponent<TTOT::Components::ManaComponent>()
              ->GetMaxMP()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Str:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetSTRStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Dex:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetDEXStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Int:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetINTStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Wis:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetWISStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Cha:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetCHAStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Def:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetDEFStat()
              .AddStatModifier(event.mod);
          break;
        case TTOT::Core::Events::StatType::Spd:
          entity->GetComponent<TTOT::Components::StatComponent>()
              ->GetSPDStat()
              .AddStatModifier(event.mod);
          break;
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::ChangeHealthEvent>(
      [this](const TTOT::Core::Events::ChangeHealthEvent &event) {
        auto entity = this->GetEntity(event.entityId);
        if (entity == nullptr)
          return;
        if (auto *healthComp =
                entity->GetComponent<TTOT::Components::HealthComponent>()) {
          healthComp->ModifyHP(event.delta);
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::ChangeManaEvent>(
      [this](const TTOT::Core::Events::ChangeManaEvent &event) {
        auto entity = this->GetEntity(event.entityId);
        if (entity == nullptr)
          return;
        if (auto *manaComp =
                entity->GetComponent<TTOT::Components::ManaComponent>()) {
          manaComp->ModifyMP(event.delta);
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::PlayerGetEvent>(
      [this](const TTOT::Core::Events::PlayerGetEvent &event) {
        if (this->player != nullptr)
          event.onLoaded(*(this->player));
      });
  eventBus.Subscribe<TTOT::Core::Events::SaveRequestEvent>(
      [this](const TTOT::Core::Events::SaveRequestEvent &event) mutable {
        TTOT::Core::MasterSaveData saveData;
        nlohmann::json j;
        this->player->Serialize(j);
        saveData.slotIndex = event.slotIndex;
        saveData.playerData = j;
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::tm localTm;
        localtime_s(&localTm, &now_time);
        std::stringstream ss;
        ss << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S");
        saveData.saveTime = ss.str();
        saveData.currentFloor = 1;

        this->saveManager->OnSaveRequest(event.slotIndex, saveData,
                                         *(this->context));
      });
  eventBus.Subscribe<TTOT::Core::Events::LoadRequestEvent>(
      [this](const TTOT::Core::Events::LoadRequestEvent &event) mutable {
        this->saveManager->OnLoadRequest(event.slotIndex, *(this->context));
      });
  eventBus.Subscribe<TTOT::Core::Events::LoadCompleteEvent>(
      [this](const TTOT::Core::Events::LoadCompleteEvent &event) mutable {
        if (event.success) {
          if (this->player == nullptr) {
            this->player =
                std::make_unique<TTOT::Entities::Player>(0, "", false);
            this->player->AddComponent(
                std::make_unique<TTOT::Components::HealthComponent>(10));
            this->player->AddComponent(
                std::make_unique<TTOT::Components::ManaComponent>(10));
            this->player->AddComponent(
                std::make_unique<TTOT::Components::MoneyComponent>(0));

            auto stat =
                std::make_unique<TTOT::Components::StatComponentBuilder>();
            stat->SetSTR(0);
            stat->SetDEX(0);
            stat->SetINT(0);
            stat->SetWIS(0);
            stat->SetCHA(0);
            stat->SetDEF(0);
            stat->SetSPD(0);
            this->player->AddComponent(std::move(stat->Build()));

            auto defaultClass = std::make_unique<TTOT::Class::ClassBase>();
            this->player->AddComponent(
                std::make_unique<TTOT::Components::ClassComponent>(
                    std::move(defaultClass)));
          }
          this->player->Deserialize(event.saveData.playerData);
          entities[this->player->GetId()] = this->player.get();
          if (nextEntityId <= this->player->GetId()) {
            nextEntityId = this->player->GetId() + 1;
          }
        }
      });
  eventBus.Subscribe<TTOT::Core::Events::InjectPurposeEvent>(
      [this](const TTOT::Core::Events::InjectPurposeEvent &event) {
        this->player->SetPurpose(event.purpose);
        this->player->SetGuidance(event.guidance);
      });
}

uint32_t TTOT::Core::GameManager::GameManager::RegistryEntity(
    TTOT::Entities::Entity *entity) {
  uint32_t id = nextEntityId;
  entities[id] = entity;
  nextEntityId++;
  return id;
}
void TTOT::Core::GameManager::GameManager::UnregistryEntity(uint32_t entityId) {
  if (auto it = entities.find(entityId); it != entities.end()) {
    entities.erase(it);
  }
}
TTOT::Entities::Entity *
TTOT::Core::GameManager::GameManager::GetEntity(uint32_t entityId) {
  if (auto it = entities.find(entityId); it != entities.end()) {
    return it->second;
  }
  return nullptr;
}
void GameManager::Run() {
  isRunning = true;
  gameState = TTOT::Core::GameState::Title;
  sceneManager->RegisterScene<TTOT::Core::Scenes::TitleScene>(0);
  sceneManager->RegisterScene<TTOT::Core::Scenes::SettingScene>(1);
  sceneManager->RegisterScene<TTOT::Core::Scenes::IntroScene>(2);
  sceneManager->RegisterScene<TTOT::Core::Scenes::FieldScene>(3);
  sceneManager->RegisterScene<TTOT::Core::Scenes::SaveFileLoadScene>(4);
  this->currentScene = sceneManager->LoadScene(0, *context);
  context->eventBus.Publish(TTOT::Core::Events::VolumeChangeEvent{
      TTOT::Core::Events::VolumeType::Master, 0});
  auto input_handler = ftxui::CatchEvent(
      ftxui::Make<ftxui::ComponentBase>(), [this](const ftxui::Event &event) {
        if (this->sceneManager->GetCurrentScene() != nullptr) {
          this->currentScene->HandleInput(event);
          return true;
        }
        return false;
      });
  auto renderer = ftxui::Renderer(input_handler, [this]() {
    if (this->nextSceneId != -1) {
      if (this->currentScene != nullptr) {
        this->currentScene->OnExit();
      }

      this->currentScene =
          sceneManager->LoadScene(this->nextSceneId, *(this->context));
      this->nextSceneId = -1;

      if (this->currentScene != nullptr) {
        this->currentScene->OnEnter();
      }
    }
    gameState = TTOT::Core::GameState::GameLoop;
    if (this->currentScene != nullptr) {
      this->currentScene->Update();
      return this->currentScene->Render();
    }
    return ftxui::text("씬을 불러오는데 실패했습니다") | ftxui::border;
  });

  screen.Loop(renderer);
}
} // namespace TTOT::Core