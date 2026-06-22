#pragma once
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <string>
namespace TTOT::Core
{
    struct MasterSaveData
    {
        int slotIndex = 1;
        std::string saveTime;
        int currentFloor;
        nlohmann::json playerData;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MasterSaveData, slotIndex, saveTime, currentFloor, playerData);
}