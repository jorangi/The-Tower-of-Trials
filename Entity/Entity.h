#pragma once
#include "Class/ClassBase.h"
#include "Component/ClassComponent.h"
#include "Component/Components.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Core/ISaveData.h"
#include "Stat/StatModifier.h"
#include "Utility/Action.h"
#include <algorithm>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTOT::Entities {
class Entity : public Core::ISaveData {
private:
  struct ComponentNode {
    uint32_t id;
    uint32_t typeID;
    std::unique_ptr<Components::Component> component;
  };
  Utilities::Action<std::string> OnNameChanged;
  Utilities::Action<bool> OnGenderChanged;

protected:
  uint32_t _id;
  std::string _name;
  bool gender; // f: male, t: female
  uint32_t _next_id = 0;
  std::vector<Components::Component *> _compArr;
  std::vector<ComponentNode> _comps;
  std::unique_ptr<TTOT::Class::ClassBase> classInfo;

public:
  Entity(uint32_t id, const std::string &name = "");
  virtual ~Entity() = default;
  virtual void Serialize(nlohmann::json &j) const override {
    j["id"] = _id;
    j["name"] = _name;
    j["gender"] = gender;

    if (auto *classComp = GetComponent<TTOT::Components::ClassComponent>()) {
      if (classComp->GetClassInfo()) {
        classComp->GetClassInfo()->Serialize(j["classData"]);
      }
    }
    if (auto *healthComp = GetComponent<TTOT::Components::HealthComponent>()) {
      healthComp->Serialize(j["HP"]);
    }

    if (auto *manaComp = GetComponent<TTOT::Components::ManaComponent>()) {
      manaComp->Serialize(j["MP"]);
    }
    if (auto *moneyComp = GetComponent<TTOT::Components::MoneyComponent>()) {
      moneyComp->Serialize(j["Money"]);
    }
    if (auto *statComp = GetComponent<TTOT::Components::StatComponent>()) {
      statComp->Serialize(j["stats"]);
    }
  }
  virtual void Deserialize(const nlohmann::json &j) override {
    j.at("id").get_to(_id);
    j.at("name").get_to(_name);
    OnNameChanged.Invoke(_name);
    j.at("gender").get_to(gender);
    OnGenderChanged.Invoke(gender);

    if (j.contains("HP")) {
      if (auto *healthComp =
              GetComponent<TTOT::Components::HealthComponent>()) {
        healthComp->Deserialize(j.at("HP"));
      }
    }
    if (j.contains("MP")) {
      if (auto *manaComp = GetComponent<TTOT::Components::ManaComponent>()) {
        manaComp->Deserialize(j.at("MP"));
      }
    }
    if (j.contains("Money")) {
      if (auto *moneyComp = GetComponent<TTOT::Components::MoneyComponent>()) {
        moneyComp->Deserialize(j.at("Money"));
      }
    }
    if (j.contains("stats")) {
      if (auto *statComp = GetComponent<TTOT::Components::StatComponent>()) {
        const auto &s = j["stats"];
        statComp->Deserialize(s);
      }
    }

    if (j.contains("classData")) {
      if (auto *classComp = GetComponent<TTOT::Components::ClassComponent>()) {
        if (classComp->GetClassInfo()) {
          auto *classInfo = classComp->GetClassInfo();
          classInfo->Deserialize(j.at("classData"));

          if (auto *healthComp =
                  GetComponent<TTOT::Components::HealthComponent>()) {
            healthComp->GetMaxHP().AddStatModifier(TTOT::Stats::StatModifier(
                static_cast<int>(classInfo->GetHp()), 10000,
                TTOT::Stats::StatModifierType::Flat));
          }
          if (auto *manaComp =
                  GetComponent<TTOT::Components::ManaComponent>()) {
            manaComp->GetMaxMP().AddStatModifier(TTOT::Stats::StatModifier(
                static_cast<int>(classInfo->GetMp()), 10000,
                TTOT::Stats::StatModifierType::Flat));
          }
          if (auto *statComp =
                  GetComponent<TTOT::Components::StatComponent>()) {
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
  void AddComponent(std::unique_ptr<Components::Component> comp) {
    if (!comp)
      return;
    uint32_t comp_id = _next_id++;
    uint32_t typeID = comp->GetTypeID();
    if (typeID >= _compArr.size()) {
      _compArr.resize(typeID + 1, nullptr);
    }
    _compArr[typeID] = comp.get();
    _comps.push_back({comp_id, typeID, std::move(comp)});
  }
  template <typename T> T *GetComponent() const {
    uint32_t typeID = Components::GetComponentTypeID<T>();
    if (typeID >= _compArr.size())
      return nullptr;
    return static_cast<T *>(_compArr[typeID]);
  }
  void RemoveComponent(uint32_t id) {
    auto it =
        std::find_if(_comps.begin(), _comps.end(),
                     [id](const ComponentNode &node) { return node.id == id; });
    if (it != _comps.end()) {
      uint32_t typeID = it->typeID;
      if (typeID < _compArr.size()) {
        _compArr[typeID] = nullptr;
      }
      _comps.erase(it);
    }
  }
  Utilities::Action<std::string> &GetNameChanged() { return OnNameChanged; }
  Utilities::Action<bool> &GetGenderChanged() { return OnGenderChanged; }

  virtual uint32_t GetId() const { return _id; }
  virtual std::string GetName() const { return _name; }
  const bool GetGender() const { return gender; }
};
} // namespace TTOT::Entities