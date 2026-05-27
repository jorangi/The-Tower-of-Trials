#include <iostream>
#include "Core/SceneManager.h"

namespace TTOT::Core
{
    void SceneManager::LoadScene(int sceneId)
    {
        auto it = sceneMap.find(sceneId);
        if (it != sceneMap.end())
        {
            //TODO: 씬 로드 로직 구현
        }
        else
        {
            std::cerr << sceneId << "에 해당하는 씬이 존재하지 않습니다." << std::endl;
        }
    }
    SceneManager& SceneManager::Inst()
    {
        static SceneManager instance;
        return instance;
    }
}
