#include "Core/GameManager.h"

namespace TTOT::Core
{
    GameManager::GameManager()
    {
        isRunning = false;
        gameState = TTOT::Core::GameState::None;
    }
    void GameManager::Run()
    {
        gemini = std::make_unique<TTOT::Network::ConnGemini>();
        sceneManager = std::make_unique<TTOT::Core::SceneManager>();
        soundManager = std::make_unique<TTOT::Utilities::SoundManager>();
        
        isRunning = true;
        gameState = TTOT::Core::GameState::Init;
        while(isRunning)
        {
            gameState = TTOT::Core::GameState::GameLoop;
            break;
        }
    }
}