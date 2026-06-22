#pragma once
#include <nlohmann/json_fwd.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <memory>
#include <algorithm>
#include "Component/ClassComponent.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Component/Components.h"
#include "Class/ClassBase.h"
#include "Core/ISaveData.h"

namespace TTOT::Entities
{
    class Entity: public Core::ISaveData
    {
        private:
            struct ComponentNode
            {
                uint32_t id;
                uint32_t typeID;
                std::unique_ptr<Components::Component> component;
            };
        protected:
            uint32_t _id;
            std::string _name;
            bool gender; //f: male, t: female
            uint32_t _next_id = 0;
            std::vector<Components::Component*> _compArr;
            std::vector<ComponentNode> _comps;
            std::unique_ptr<TTOT::Class::ClassBase> classInfo;
        public:
            Entity(uint32_t id, const std::string& name="");
            virtual ~Entity() = default;
            virtual void Serialize(nlohmann::json& j) const override
            {
                j["id"] = _id;
                j["name"] = _name;
                j["gender"] = gender;

                if (auto* classComp = GetComponent<TTOT::Components::ClassComponent>())
                {
                    if (classComp->GetClassInfo())
                    {
                        classComp->GetClassInfo()->Serialize(j["classData"]);
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
                    j["stats"] = {
                        {"str", statComp->GetSTR()},
                        {"dex", statComp->GetDEX()},
                        {"int", statComp->GetINT()},
                        {"wis", statComp->GetWIS()},
                        {"cha", statComp->GetCHA()},
                        {"def", statComp->GetDEF()},
                        {"spd", statComp->GetSPD()}
                    };
                }
            }
            virtual void Deserialize(const nlohmann::json& j) override
            {
                j.at("id").get_to(_id);
                j.at("name").get_to(_name);
                j.at("gender").get_to(gender);

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
                        if (classComp->GetClassInfo()) classComp->GetClassInfo()->Deserialize(j.at("classData"));
                    }
                }
            }
            void AddComponent(std::unique_ptr<Components::Component> comp)
            {
                if(!comp) return;
                uint32_t comp_id = _next_id++;
                uint32_t typeID = comp->GetTypeID();
                if(typeID >= _compArr.size())
                {
                    _compArr.resize(typeID + 1, nullptr);
                }
                _compArr[typeID] = comp.get();
                _comps.push_back({comp_id, typeID, std::move(comp)});
            }
            template<typename T>
            T* GetComponent() const
            {
                uint32_t typeID = Components::GetComponentTypeID<T>();
                if(typeID >= _compArr.size()) return nullptr;
                return static_cast<T*>(_compArr[typeID]);
            }
            void RemoveComponent(uint32_t id)
            {
                auto it = std::find_if(_comps.begin(), _comps.end(), [id](const ComponentNode& node)
                {
                    return node.id==id;
                });
                if(it != _comps.end())
                {
                    uint32_t typeID = it->typeID;
                    if(typeID < _compArr.size())
                    {
                        _compArr[typeID] = nullptr;
                    }
                    _comps.erase(it);
                }
            }
            virtual uint32_t GetId() const {return _id;}
            virtual std::string GetName() const {return _name;}
            const bool GetGender() const {return gender;}
    };
}