#pragma once
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <string>
namespace TTOT::Core {
struct MasterSaveData {
  int slotIndex = 1;
  std::string saveTime;
  int currentFloor;
  unsigned int mapSeed = 0;
  int playerX = -1;
  int playerY = -1;
  nlohmann::json playerData;
  nlohmann::json worldData;
  nlohmann::json classData;
  nlohmann::json entityData;
  nlohmann::json itemData;
  nlohmann::json skillData;
  nlohmann::json objectState;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MasterSaveData, slotIndex, saveTime,
                                   currentFloor, mapSeed, playerX, playerY,
                                   playerData);
} // namespace TTOT::Core