#pragma once
#include <map>


namespace TTOT::Core
{
    class SceneManager
    {
        private:
            SceneManager(const SceneManager&) = delete;
            void operator=(const SceneManager&) = delete;
            std::map<int, void(*)()> sceneMap; 
        public:
            SceneManager() {}
            void LoadScene(int sceneId);
    };
}