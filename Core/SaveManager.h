#pragma once
#include "Core/GameContext.h"
#include "Core/MasterSaveData.h"
namespace TTOT::Core
{
    class SaveManager
    {
        public:
            void OnSaveRequest(int slotIndex, MasterSaveData saveData, GameContext& context);
            void OnLoadRequest(int slotIndex, GameContext& context);
    };
}