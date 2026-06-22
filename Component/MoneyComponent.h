#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include "Core/ISaveData.h"
#include <nlohmann/json.hpp>
#include "Component/Components.h"
#include "Utility/Action.h"

namespace TTOT::Components
{
    class MoneyComponent : public Component, public Core::ISaveData
    {
        private:
            TTOT::Utilities::Action<int> OnMoneyChanged;
            uint32_t money;
        public:
            MoneyComponent(uint32_t init): money(init){}
            uint32_t GetTypeID() const override {return GetComponentTypeID<MoneyComponent>();}
            std::string ToString() const override
            {
                std::ostringstream oss;
                oss << "소지금 : " << money << "G";
                return oss.str();
            }
            void GainMoney(uint32_t amount)
            {
                money += amount;
                OnMoneyChanged.Invoke(money);
            }
            void LostMoney(uint32_t amount)
            {
                money = max(0, static_cast<int>(money-amount));
                OnMoneyChanged.Invoke(money);
            }
            uint32_t GetMoney() const
            {
                return money;
            }
            void Serialize(nlohmann::json& j) const override
            {
                j["money"] = money;
            }
            void Deserialize(const nlohmann::json& j) override
            {
                j.at("money").get_to(money);
            }
    };
}