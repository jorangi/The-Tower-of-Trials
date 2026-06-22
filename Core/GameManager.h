#pragma once
#include "Core/GameContext.h"
#include "Core/SaveManager.h"
#include "Core/SceneManager.h"
#include "Core/EventBus.h"
#include "Utility/SoundManager.h"
#include "Network/ConnGemini.h"
#include <memory>
#include <ftxui/component/screen_interactive.hpp>
#include "Entity/PlayerFactory.h"
#include "Entity/MonsterFactory.h"


namespace TTOT::Core
{
    enum class GameState
    {
        None,
        Init,
        Title,
        Intro,
        GameLoop,
        Exit
    };
    class GameManager
    {
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
            
            TTOT::Core::Scenes::IScene* currentScene = nullptr;
            int nextSceneId = -1;

            std::unique_ptr<TTOT::Entities::PlayerFactory> playerFactory;
            std::unique_ptr<TTOT::Entities::Player> player;
            std::unique_ptr<TTOT::Entities::MonsterFactory> monsterFactory;
        public:
            GameManager();
            void Run();
    };
}