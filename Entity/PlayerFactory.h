#pragma once
#include "Entity/Player.h"
#include "Datas/EntityDTO.h"


namespace TTOT::Entities
{
    class PlayerFactory
    {
        public:
            static std::unique_ptr<TTOT::Entities::Player> CreatePlayer(TTOT::Datas::EntityDTO&& dto, const bool gender = false);
    };
}