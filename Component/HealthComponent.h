#pragma once
#include "Component/Components.h"
#include "Core/ISaveData.h"
#include "Stat/ModifiableStat.h"
#include "Utility/Action.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <sstream>

namespace TTOT::Components {
class HealthComponent : public Component, public Core::ISaveData {
private:
  TTOT::Utilities::Action<int> OnHealthChanged;
  TTOT::Stats::ModifiableStat _maxHP;
  int currentHP = 0;

public:
  HealthComponent(int initHP) : _maxHP(initHP), currentHP(initHP) {}
  uint32_t GetTypeID() const override {
    return GetComponentTypeID<HealthComponent>();
  }
  std::string ToString() const override {
    ostringstream oss;
    oss << "HP: " << currentHP << " / " << _maxHP.GetValue();
    return oss.str();
  }
  void TakeDamage(int damage) {
    currentHP -= damage;
    OnHealthChanged.Invoke(currentHP);
  }
  void ModifyHP(int delta) {
    currentHP += delta;
    int maxHP = _maxHP.GetValue();
    if (currentHP > maxHP)
      currentHP = maxHP;
    if (currentHP < 0)
      currentHP = 0;
    OnHealthChanged.Invoke(currentHP);
  }
  TTOT::Stats::ModifiableStat &GetMaxHP() { return _maxHP; }
  int GetCurrentHP() const { return currentHP; }
  TTOT::Utilities::Action<int> &GetOnHealthChanged() { return OnHealthChanged; }
  void Serialize(nlohmann::json &j) const override {
    j["currentHP"] = currentHP;
    j["baseMaxHP"] = _maxHP.GetBaseValue();
  }
  void Deserialize(const nlohmann::json &j) override {
    j.at("currentHP").get_to(currentHP);
    int baseMaxHP = 0;
    j.at("baseMaxHP").get_to(baseMaxHP);
    _maxHP = TTOT::Stats::ModifiableStat(baseMaxHP, _maxHP.GetStatName());
    OnHealthChanged.Invoke(currentHP);
  }
};
} // namespace TTOT::Components