#include <string>
#include <ostream>
#include "Stat/ModifiableStat.h"
#include "Utility/Action.h"

using namespace std;

namespace TTOT::Components
{
    struct Component
    {
        virtual ~Component() = default;
        public:
            virtual string ToString() const = 0;
    };
    struct HealthComponent : public Component
    {
        private:
            TTOT::Utilities::Action<int> OnHealthChanged;
            TTOT::Stats::ModifiableStat _maxHP;
            int currentHP;
        
        public:
            HealthComponent(int initHP):_maxHP(initHP), currentHP(initHP){}
            string ToString() const override
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
    struct MoneyComponent : public Component
    {
        private:
            TTOT::Utilities::Action<int> OnMoneyChanged;
            uint32_t money;
        public:
            MoneyComponent(uint32_t init): money(init){}
            string ToString() const override
            {
                ostringstream oss;
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
    };
}