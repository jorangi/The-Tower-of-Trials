#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include "Stat/ModifiableStat.h"
#include "Component/Components.h"
#include "Utility/Action.h"

namespace TTOT::Components
{
    class ManaComponent : public Component
    {
        private:
            TTOT::Utilities::Action<int> OnManaChanged;
            TTOT::Stats::ModifiableStat _maxMP;
            int currentMP;
        
        public:
            ManaComponent(int initMP):_maxMP(initMP), currentMP(initMP){}
            uint32_t GetTypeID() const override {return GetComponentTypeID<ManaComponent>();}
            std::string ToString() const override
            {
                ostringstream oss;
                oss << "MP: " << currentMP << " / " << _maxMP.GetValue();
                return oss.str();
            }
            void ConsumeMana(int val)
            {
                currentMP -= val;
                OnManaChanged.Invoke(currentMP);
            }
    };
}