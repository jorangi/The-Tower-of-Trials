#pragma once
#include "Entity.h"

namespace TTOT::Entities
{
    class Player : public Entity
    {
        public:
            Player(const uint32_t id, const std::string& name, const bool gender = false): Entity(id, name){}
            std::string ToString() const override;
            void ShowInfo() const;
    };
}