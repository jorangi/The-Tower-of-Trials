#pragma once
#include "Core/Scene/IScene.h"
#include <map>
#include <functional>
#include "Core/GameContext.h"
#include <memory>


namespace TTOT::Core
{
    class SceneManager
    {
        private:
            SceneManager(const SceneManager&) = delete;
            void operator=(const SceneManager&) = delete;
            std::map<int, std::function<std::unique_ptr<TTOT::Core::Scenes::IScene>(GameContext&)>> sceneMap; 
            std::unique_ptr<TTOT::Core::Scenes::IScene> currentScene;
        public:
            SceneManager() = default;
            template<typename T>
            void RegisterScene(int sceneId)
            {
                sceneMap[sceneId] = [](GameContext& context){
                    return std::make_unique<T>(context);
                };
            }
            std::unique_ptr<TTOT::Core::Scenes::IScene> LoadScene(int sceneId, TTOT::Core::GameContext& context);
            
    };
}