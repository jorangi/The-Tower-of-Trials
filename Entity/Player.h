#pragma once
#include "Entity.h"

namespace TTOT::Entities
{
    class Player : public Entity
    {
        public:
            Player(uint32_t id, const std::string& name): Entity(id, name){}
            std::string ToString() const override;
            void ShowInfo();
    };
}