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
class ManaComponent : public Component, public Core::ISaveData {
private:
  TTOT::Utilities::Action<int> OnManaChanged;
  TTOT::Stats::ModifiableStat _maxMP;
  int currentMP = 0;

public:
  ManaComponent(int initMP) : _maxMP(initMP), currentMP(initMP) {}
  uint32_t GetTypeID() const override {
    return GetComponentTypeID<ManaComponent>();
  }
  std::string ToString() const override {
    ostringstream oss;
    oss << "MP: " << currentMP << " / " << _maxMP.GetValue();
    return oss.str();
  }
  void ConsumeMana(int val) {
    currentMP -= val;
    OnManaChanged.Invoke(currentMP);
  }
  void ModifyMP(int delta) {
    currentMP += delta;
    int maxMP = _maxMP.GetValue();
    if (currentMP > maxMP)
      currentMP = maxMP;
    if (currentMP < 0)
      currentMP = 0;
    OnManaChanged.Invoke(currentMP);
  }

  TTOT::Stats::ModifiableStat &GetMaxMP() { return _maxMP; }
  int GetCurrentMP() const { return currentMP; }
  TTOT::Utilities::Action<int> &GetOnManaChanged() { return OnManaChanged; }
  void Serialize(nlohmann::json &j) const override {
    j["currentMP"] = currentMP;
    j["baseMaxMP"] = _maxMP.GetBaseValue();
  }
  void Deserialize(const nlohmann::json &j) override {
    j.at("currentMP").get_to(currentMP);
    int baseMaxMP = 0;
    j.at("baseMaxMP").get_to(baseMaxMP);
    _maxMP = TTOT::Stats::ModifiableStat(baseMaxMP, _maxMP.GetStatName());
    OnManaChanged.Invoke(currentMP);
  }
};
} // namespace TTOT::Components