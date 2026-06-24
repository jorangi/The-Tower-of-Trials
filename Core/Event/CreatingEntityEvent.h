#pragma once
#include "Datas/EntityDTO.h"
#include <stdint.h>
#include <string>

namespace TTOT::Core::Events {
enum class EntityType {
  Player,
  Monster
  // NPC
};
struct CreatingEntityEvent {
  uint32_t *entityId;
  std::string entityName;
  bool gender = false;
  TTOT::Core::Events::EntityType entityType;
  mutable TTOT::Datas::EntityDTO dto;
};
} // namespace TTOT::Core::Events