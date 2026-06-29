#include "Entity.h"
#include "Class/ClassBase.h"
#include "Datas/ClassDTO.h"
#include "Component/ClassComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Stat/StatModifier.h"
#include <algorithm>
#include <nlohmann/json.hpp>

namespace TTOT::Entities
{ 
    Entity::Entity(uint32_t id, const std::string& name)
        : _id(id), _name(name.empty() ? std::to_string(id) : name) {}

    Entity::~Entity() = default;

    void Entity::Serialize(nlohmann::json& j) const
    {
        j["id"] = _id;
        j["name"] = _name;
        j["gender"] = gender;

        if (auto* classComp = GetComponent<TTOT::Components::ClassComponent>())
        {
            if (classComp->GetClassInfo())
            {
                j["classData"] = classComp->GetClassInfo()->ToDTO();
            }
        }
        if (auto* healthComp = GetComponent<TTOT::Components::HealthComponent>())
        {
            healthComp->Serialize(j["HP"]);
        }

        if (auto* manaComp = GetComponent<TTOT::Components::ManaComponent>())
        {
            manaComp->Serialize(j["MP"]);
        }
        if (auto* moneyComp = GetComponent<TTOT::Components::MoneyComponent>())
        {
            moneyComp->Serialize(j["Money"]);
        }
        if (auto* statComp = GetComponent<TTOT::Components::StatComponent>())
        {
            statComp->Serialize(j["stats"]);
        }
    }

    void Entity::Deserialize(const nlohmann::json& j)
    {
        j.at("id").get_to(_id);
        j.at("name").get_to(_name);
        OnNameChanged.Invoke(_name);
        j.at("gender").get_to(gender);
        OnGenderChanged.Invoke(gender);

        if (j.contains("HP"))
        {
            if (auto* healthComp = GetComponent<TTOT::Components::HealthComponent>())
            {
                healthComp->Deserialize(j.at("HP"));
            }
        }
        if (j.contains("MP"))
        {
            if (auto* manaComp = GetComponent<TTOT::Components::ManaComponent>())
            {
                manaComp->Deserialize(j.at("MP"));
            }
        }
        if (j.contains("Money"))
        {
            if (auto* moneyComp = GetComponent<TTOT::Components::MoneyComponent>())
            {
                moneyComp->Deserialize(j.at("Money"));
            }
        }
        if (j.contains("stats"))
        {
            if (auto* statComp = GetComponent<TTOT::Components::StatComponent>())
            {
                const auto& s = j["stats"];
                statComp->Deserialize(s);
            }
        }

        if (j.contains("classData"))
        {
            if (auto* classComp = GetComponent<TTOT::Components::ClassComponent>())
            {
                if (classComp->GetClassInfo())
                {
                    auto classDto = j.at("classData").get<TTOT::Datas::ClassDTO>();
                    *classComp->GetClassInfo() = TTOT::Class::ClassBase(classDto);
                    auto* classInfo = classComp->GetClassInfo();

                    if (auto* healthComp = GetComponent<TTOT::Components::HealthComponent>())
                    {
                        healthComp->GetMaxHP().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetHp()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                    }
                    if (auto* manaComp = GetComponent<TTOT::Components::ManaComponent>())
                    {
                        manaComp->GetMaxMP().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetMp()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                    }
                    if (auto* statComp = GetComponent<TTOT::Components::StatComponent>())
                    {
                        statComp->GetSTRStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetStr()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetDEXStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetDex()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetINTStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetInt()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetWISStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetWis()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetCHAStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetCha()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetDEFStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetDef()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                        statComp->GetSPDStat().AddStatModifier(TTOT::Stats::StatModifier(
                            static_cast<int>(classInfo->GetSpd()), 10000,
                            TTOT::Stats::StatModifierType::Flat));
                    }
                }
            }
        }
    }

    void Entity::AddComponent(std::unique_ptr<Components::Component> comp)
    {
        if (!comp)
            return;
        uint32_t comp_id = _next_id++;
        uint32_t typeID = comp->GetTypeID();
        if (typeID >= _compArr.size())
        {
            _compArr.resize(typeID + 1, nullptr);
        }
        _compArr[typeID] = comp.get();
        _comps.push_back({ comp_id, typeID, std::move(comp) });
    }

    void Entity::RemoveComponent(uint32_t id)
    {
        auto it = std::find_if(_comps.begin(), _comps.end(),
            [id](const ComponentNode& node) { return node.id == id; });
        if (it != _comps.end())
        {
            uint32_t typeID = it->typeID;
            if (typeID < _compArr.size())
            {
                _compArr[typeID] = nullptr;
            }
            _comps.erase(it);
        }
    }

    Utilities::Action<std::string>& Entity::GetNameChanged()
    {
        return OnNameChanged;
    }

    Utilities::Action<bool>& Entity::GetGenderChanged()
    {
        return OnGenderChanged;
    }

    uint32_t Entity::GetId() const
    {
        return _id;
    }

    std::string Entity::GetName() const
    {
        return _name;
    }

    const bool Entity::GetGender() const
    {
        return gender;
    }
}