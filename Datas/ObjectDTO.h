#pragma once
#include <string>
#include <vector>
#include <stdint.h>
#include <nlohmann/json.hpp>

namespace TTOT::Datas {

enum class ObjectType { Chest, Door, Trap, Portal, Obstacle };
enum class TrapTriggerType { OnStep, OnInteract };

class ObjectDTO {
private:
  uint32_t objectId;
  std::string name;
  std::string desc;
  ObjectType objectType;

  // Interaction/Physics Flags
  bool isInteractable;
  bool isBlocking; // Can entities walk through it?

  // Unlock Requirements (for locked Chests / Doors)
  bool isLocked;
  uint32_t keyItemId; // ID of the item needed to unlock it

  // Chest Rewards
  uint32_t rewardMoney;
  std::vector<uint32_t> rewardItemIds;

  // Trap properties
  TrapTriggerType trapTrigger;
  int trapDamage;
  uint32_t trapStatusEffectId; // References status effect or skill ID

  // Portal destinations
  int targetFloor;
  int targetX;
  int targetY;

public:
  ObjectDTO()
      : objectId(0), objectType(ObjectType::Obstacle), isInteractable(false),
        isBlocking(true), isLocked(false), keyItemId(0), rewardMoney(0),
        trapTrigger(TrapTriggerType::OnStep), trapDamage(0), trapStatusEffectId(0),
        targetFloor(0), targetX(-1), targetY(-1) {}

  ObjectDTO(uint32_t objectId, const std::string &name, const std::string &desc,
            ObjectType objectType, bool isInteractable, bool isBlocking,
            bool isLocked, uint32_t keyItemId, uint32_t rewardMoney,
            std::vector<uint32_t> rewardItemIds, TrapTriggerType trapTrigger,
            int trapDamage, uint32_t trapStatusEffectId, int targetFloor,
            int targetX, int targetY)
      : objectId(objectId), name(name), desc(desc), objectType(objectType),
        isInteractable(isInteractable), isBlocking(isBlocking), isLocked(isLocked),
        keyItemId(keyItemId), rewardMoney(rewardMoney),
        rewardItemIds(rewardItemIds), trapTrigger(trapTrigger),
        trapDamage(trapDamage), trapStatusEffectId(trapStatusEffectId),
        targetFloor(targetFloor), targetX(targetX), targetY(targetY) {}

  uint32_t GetObjectId() const { return objectId; }
  std::string GetName() const { return name; }
  std::string GetDesc() const { return desc; }
  ObjectType GetObjectType() const { return objectType; }

  bool IsInteractable() const { return isInteractable; }
  bool IsBlocking() const { return isBlocking; }
  bool IsLocked() const { return isLocked; }
  uint32_t GetKeyItemId() const { return keyItemId; }

  // Chest Getters
  uint32_t GetRewardMoney() const { return rewardMoney; }
  std::vector<uint32_t> GetRewardItemIds() const { return rewardItemIds; }

  // Trap Getters
  TrapTriggerType GetTrapTrigger() const { return trapTrigger; }
  int GetTrapDamage() const { return trapDamage; }
  uint32_t GetTrapStatusEffectId() const { return trapStatusEffectId; }

  // Portal Getters
  int GetTargetFloor() const { return targetFloor; }
  int GetTargetX() const { return targetX; }
  int GetTargetY() const { return targetY; }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectDTO, objectId, name, desc, objectType,
                                 isInteractable, isBlocking, isLocked,
                                 keyItemId, rewardMoney, rewardItemIds,
                                 trapTrigger, trapDamage, trapStatusEffectId,
                                 targetFloor, targetX, targetY)
};

class ObjectDTOBuilder {
private:
  uint32_t objectId = 0;
  std::string name;
  std::string desc;
  ObjectType objectType = ObjectType::Obstacle;
  bool isInteractable = false;
  bool isBlocking = true;
  bool isLocked = false;
  uint32_t keyItemId = 0;
  uint32_t rewardMoney = 0;
  std::vector<uint32_t> rewardItemIds;
  TrapTriggerType trapTrigger = TrapTriggerType::OnStep;
  int trapDamage = 0;
  uint32_t trapStatusEffectId = 0;
  int targetFloor = 0;
  int targetX = -1;
  int targetY = -1;

public:
  ObjectDTOBuilder() = default;

  ObjectDTOBuilder& ObjectId(uint32_t objectId) { this->objectId = objectId; return *this; }
  ObjectDTOBuilder& Name(const std::string& name) { this->name = name; return *this; }
  ObjectDTOBuilder& Desc(const std::string& desc) { this->desc = desc; return *this; }
  ObjectDTOBuilder& Type(ObjectType type) { this->objectType = type; return *this; }

  // Flags
  ObjectDTOBuilder& Interactable(bool interact) { this->isInteractable = interact; return *this; }
  ObjectDTOBuilder& Blocking(bool block) { this->isBlocking = block; return *this; }
  ObjectDTOBuilder& Locked(bool lock, uint32_t keyId = 0) {
    this->isLocked = lock;
    this->keyItemId = keyId;
    return *this;
  }

  // Chest rewards
  ObjectDTOBuilder& Reward(uint32_t money, const std::vector<uint32_t>& items) {
    this->rewardMoney = money;
    this->rewardItemIds = items;
    return *this;
  }

  // Trap details
  ObjectDTOBuilder& Trap(TrapTriggerType trigger, int dmg, uint32_t effectId = 0) {
    this->trapTrigger = trigger;
    this->trapDamage = dmg;
    this->trapStatusEffectId = effectId;
    return *this;
  }

  // Portal details
  ObjectDTOBuilder& Portal(int floor, int tx, int ty) {
    this->targetFloor = floor;
    this->targetX = tx;
    this->targetY = ty;
    return *this;
  }

  ObjectDTO Build() {
    return ObjectDTO(objectId, name, desc, objectType, isInteractable,
                     isBlocking, isLocked, keyItemId, rewardMoney,
                     rewardItemIds, trapTrigger, trapDamage, trapStatusEffectId,
                     targetFloor, targetX, targetY);
  }
};

} // namespace TTOT::Datas
