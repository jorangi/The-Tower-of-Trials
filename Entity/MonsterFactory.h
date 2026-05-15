#pragma once
#include "Monster.h"
#include "Datas/EntityDTO.h"

namespace TTOT::Entities
{
    class MonsterFactory
    {
        public:
            static unique_ptr<Monster> CreateMonster(TTOT::Datas::EntityDTO& dto)
            {
                auto monster = std::make_unique<Monster>(dto.GetId(), dto.GetName());

                auto health = std::make_unique<Components::HealthComponent>(dto.GetHP());
                auto money = std::make_unique<Components::MoneyComponent>(dto.GetMoney());

                monster->AddComponent(std::move(health));
                monster->AddComponent(std::move(money));

                return monster;
            }
    };
}