#include <iostream>
#include "Core/SceneManager.h"
#include "Utility/ScreenUtility.h"

namespace TTOT::Core
{
    std::unique_ptr<TTOT::Core::Scenes::IScene> SceneManager::LoadScene(int sceneId, TTOT::Core::GameContext& context)
    {
        auto it = sceneMap.find(sceneId);
        if (it != sceneMap.end())
        {
            TTOT::Utilities::ScreenUtility::Clear();
            currentScene = it->second(context);
            currentScene->OnEnter();
            return std::move(currentScene);
        }
        else
        {
            std::cerr << sceneId << "에 해당하는 씬이 존재하지 않습니다." << std::endl;
            return nullptr;
        }
    }
}
