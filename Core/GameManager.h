#pragma once
#include "Core/EventBus.h"
#include "Core/GameContext.h"
#include "Core/SaveManager.h"
#include "Core/SceneManager.h"
#include "Entity/Entity.h"
#include "Entity/MonsterFactory.h"
#include "Entity/PlayerFactory.h"
#include "Network/ConnGemini.h"
#include "Utility/SoundManager.h"
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <unordered_map>

namespace TTOT::Core {
enum class GameState { None, Init, Title, Intro, GameLoop, Exit };
class GameManager {
private:
  bool isRunning;
  TTOT::Core::GameState gameState;
  TTOT::Core::EventBus eventBus;

  std::unique_ptr<TTOT::Network::ConnGemini> gemini;
  std::unique_ptr<TTOT::Core::SceneManager> sceneManager;
  std::unique_ptr<TTOT::Utilities::SoundManager> soundManager;
  std::unique_ptr<TTOT::Utilities::ImageRenderer> imageRenderer;
  std::unique_ptr<TTOT::Core::SaveManager> saveManager;
  std::unique_ptr<TTOT::Core::GameContext> context;

  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

  TTOT::Core::Scenes::IScene *currentScene = nullptr;
  int nextSceneId = -1;

  std::unordered_map<uint32_t, TTOT::Entities::Entity *> entities;
  std::unique_ptr<TTOT::Entities::PlayerFactory> playerFactory;
  std::unique_ptr<TTOT::Entities::Player> player;
  std::unique_ptr<TTOT::Entities::MonsterFactory> monsterFactory;

  uint32_t nextEntityId = 0;
  uint32_t RegistryEntity(TTOT::Entities::Entity *entity);
  void UnregistryEntity(uint32_t entityId);
  TTOT::Entities::Entity *GetEntity(uint32_t entityId);

public:
  GameManager();
  void Run();
};
} // namespace TTOT::Core