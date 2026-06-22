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
    class ManaComponent : public Component, public Core::ISaveData
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
            
            TTOT::Stats::ModifiableStat GetMaxMP()
            {
                return _maxMP;
            }
            int GetCurrentMP() const {return currentMP;}
            void Serialize(nlohmann::json& j) const override
            {
                j["currentMP"] = currentMP;
                j["baseMaxMP"] = _maxMP.GetBaseValue();
            }
            void Deserialize(const nlohmann::json& j) override
            {
                j.at("currentMP").get_to(currentMP);
                int baseMaxMP = 0;
                j.at("baseMaxMP").get_to(baseMaxMP);
                _maxMP = TTOT::Stats::ModifiableStat(baseMaxMP, _maxMP.GetStatName());
            }
    };
}