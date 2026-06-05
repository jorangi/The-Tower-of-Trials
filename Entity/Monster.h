#pragma once
#include "Entity.h"
#include <iostream>

namespace TTOT::Entities
{
    class Monster : public Entity
    {
        public:
            Monster(uint32_t id, const std::string& name): Entity(id, name){}
            std::string ToString() const
            {
                std::ostringstream oss;
                oss << "┌──────────────────────────────────────┐" << std::endl;
                oss << "   id: " << this->GetId() << std::endl;
                oss << "   name: " << this->GetName() << std::endl;
                oss << " ======================================" << std::endl;
                oss << "    " << this->GetComponent<Components::HealthComponent>()->ToString() << std::endl;
                // oss << "   mp: " << mp << std::endl;
                // oss << "   str: " << str << std::endl;
                // oss << "   def: " << def << std::endl;
                // oss << "   spd: " << spd << std::endl;
                oss << " ======================================" << std::endl;
                oss << "    " << this->GetComponent<Components::MoneyComponent>()->ToString() << std::endl;
                // oss << " ======================================" << std::endl;
                // oss << "   skills: ";
                // for (auto skill : skills)
                // {
                //     oss << skill << " ";
                // }
                oss << std::endl;
                oss << "└──────────────────────────────────────┘" << std::endl;

                return oss.str();
            }
    };
}