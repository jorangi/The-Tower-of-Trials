#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include "Stat/ModifiableStat.h"
#include "Component/Components.h"
#include "Utility/Action.h"

namespace TTOT::Components
{
    class HealthComponent : public Component
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
    };
}