#include "Entity/Player.h"
#include <iostream>
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"

namespace TTOT::Entities
{
    std::string Player::ToString() const
    {
        std::ostringstream oss;
        oss << "┌──────────────────────────────────────┐" << endl;
        oss << "   이름: " << _name << endl;
        oss << "   성별: " << (gender ? "여자" : "남자") << endl;
        oss << "========================================" << endl;
        oss << "   " << GetComponent<Components::HealthComponent>()->ToString() << endl;
        oss << "   " << GetComponent<Components::ManaComponent>()->ToString() << endl;
        oss << "   " << GetComponent<Components::StatComponent>()->ToString() << endl;
        oss << "========================================" << endl;
        oss << "   " << GetComponent<Components::MoneyComponent>()->ToString() << endl;
        oss << "└──────────────────────────────────────┘" << endl;
        return oss.str();
    }
    void Player::ShowInfo() const
    {
        std::cout << ToString() << endl;
    }
}