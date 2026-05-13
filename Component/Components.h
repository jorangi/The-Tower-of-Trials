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
}