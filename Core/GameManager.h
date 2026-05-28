#pragma once
#include "Core/SceneManager.h"
#include "Utility/SoundManager.h"
#include "Network/ConnGemini.h"
#include <memory>


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

            std::unique_ptr<TTOT::Network::ConnGemini> gemini;
            std::unique_ptr<TTOT::Core::SceneManager> sceneManager;
            std::unique_ptr<TTOT::Utilities::SoundManager> soundManager;
        public:
            GameManager();
            void Run();
    };
}