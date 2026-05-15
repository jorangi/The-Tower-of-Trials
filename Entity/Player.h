#pragma once
#include "Entity.h"
#include <iostream>

namespace TTOT::Entities
{
    class Player : public Entity
    {
        public:
            Player(uint32_t id, const std::string& name): Entity(id, name)
            {
                AddComponent(std::make_unique<Components::HealthComponent>(50));
                AddComponent(std::make_unique<Components::MoneyComponent>(0));
            }
            std::string ToString() const override
            {
                std::ostringstream oss;
                oss << "┌──────────────────────────────────────┐" << endl;
                oss << "   이름: " << _name << endl;
                oss << "========================================" << endl;
                oss << "   " << GetComponent<Components::HealthComponent>()->ToString() << endl;
                oss << "========================================" << endl;
                oss << "   " << GetComponent<Components::MoneyComponent>()->ToString() << endl;
                oss << "└──────────────────────────────────────┘" << endl;
                return oss.str();
            }
            void ShowInfo()
            {
                std::cout << ToString() << endl;
            }
    };
}