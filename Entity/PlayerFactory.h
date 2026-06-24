#pragma once
#include "Datas/EntityDTO.h"
#include "Entity/Player.h"

namespace TTOT::Entities {
class PlayerFactory {
public:
  static std::unique_ptr<TTOT::Entities::Player>
  CreatePlayer(uint32_t id, TTOT::Datas::EntityDTO &&dto,
               const bool gender = false);
};
} // namespace TTOT::Entities