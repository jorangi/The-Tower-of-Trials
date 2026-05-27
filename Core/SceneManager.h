#pragma once
#include <map>

namespace TTOT::Core
{
    class SceneManager
    {
        private:
            SceneManager() {}
            SceneManager(const SceneManager&) = delete;
            void operator=(const SceneManager&) = delete;
            std::map<int, void(*)()> sceneMap; 
        public:
            void LoadScene(int sceneId);
            static SceneManager& Inst();
    };
}