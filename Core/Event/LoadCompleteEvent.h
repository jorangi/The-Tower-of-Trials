#pragma once
#include <string>
#include "Core/MasterSaveData.h"

namespace TTOT::Core::Events
{
    struct LoadCompleteEvent
    {
        int slotIndex;
        bool success;
        TTOT::Core::MasterSaveData saveData;
        std::string errorMessage;
    };
}
