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
    void Player::SetPurpose(const std::string& purpose)
    {
        this->purpose = purpose;
    }
    void Player::SetGuidance(const std::string& guidance)
    {
        this->guidance = guidance;
    }
    void Player::SetQuestDataJson(const std::string& json)
    {
        this->questDataJson = json;
    }
    void Player::Serialize(nlohmann::json& j) const
    {
        Entity::Serialize(j);
        j["purpose"] = purpose;
        j["guidance"] = guidance;
        j["questDataJson"] = questDataJson;
    }
    void Player::Deserialize(const nlohmann::json& j)
    {
        Entity::Deserialize(j);

        if (j.contains("purpose")) j.at("purpose").get_to(purpose);
        if (j.contains("guidance"))  j.at("guidance").get_to(guidance);
        if (j.contains("questDataJson")) j.at("questDataJson").get_to(questDataJson);
    }
}