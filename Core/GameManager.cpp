#include "Core/GameManager.h"
#include "Core/Scene/TitleScene.h"
#include <iostream>

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
        
        sceneManager->RegisterScene<TTOT::Core::Scenes::TitleScene>(0);
        isRunning = true;
        gameState = TTOT::Core::GameState::Init;
        GameContext context{eventBus};
        std::unique_ptr<TTOT::Core::Scenes::IScene> currentScene = sceneManager->LoadScene(0, context);
        while(isRunning)
        {
            gameState = TTOT::Core::GameState::GameLoop;
            currentScene->Update();
            std::cout << "입력: ";
            std::string input;
            std::cin >> input;
            currentScene->HandleInput(input);
        }
    }
}