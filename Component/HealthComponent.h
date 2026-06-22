#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include "Core/ISaveData.h"
#include <nlohmann/json.hpp>
#include "Stat/ModifiableStat.h"
#include "Component/Components.h"
#include "Utility/Action.h"

namespace TTOT::Components
{
    class HealthComponent : public Component, public Core::ISaveData
    {
        private:
            TTOT::Utilities::Action<int> OnHealthChanged;
            TTOT::Stats::ModifiableStat _maxHP;
            int currentHP;
        
        public:
            HealthComponent(int initHP):_maxHP(initHP), currentHP(initHP){}
            uint32_t GetTypeID() const override {return GetComponentTypeID<HealthComponent>();}
            std::string ToString() const override
            {
                ostringstream oss;
                oss << "HP: " << currentHP << " / " << _maxHP.GetValue();
                return oss.str();
            }
            void TakeDamage(int damage)
            {
                currentHP -= damage;
                OnHealthChanged.Invoke(currentHP);
            }
            TTOT::Stats::ModifiableStat GetMaxHP()
            {
                return _maxHP;
            }
            int GetCurrentHP() const {return currentHP;}
            void Serialize(nlohmann::json& j) const override
            {
                j["currentHP"] = currentHP;
                j["baseMaxHP"] = _maxHP.GetBaseValue();
            }
            void Deserialize(const nlohmann::json& j) override
            {
                j.at("currentHP").get_to(currentHP);
                int baseMaxHP = 0;
                j.at("baseMaxHP").get_to(baseMaxHP);
                _maxHP = TTOT::Stats::ModifiableStat(baseMaxHP, _maxHP.GetStatName());
            }
    };
}