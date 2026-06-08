#pragma once
#include <stdint.h>
#include <string>
#include "Datas/EntityDTO.h"

namespace TTOT::Core::Events
{
    enum class EntityType
    {
        Player,
        Monster
        //NPC
    };
    struct CreatingEntityEvent
    {
        uint32_t entityId;
        std::string entityName;
        TTOT::Core::Events::EntityType entityType;
        const TTOT::Datas::EntityDTO dto;
    };
}