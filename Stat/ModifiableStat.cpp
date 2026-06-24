#include "ModifiableStat.h"
#include "StatModifier.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <ostream>
#include <sstream>

using namespace std;
namespace TTOT::Stats {
ModifiableStat::ModifiableStat(int baseValue, string statName)
    : _baseValue(baseValue), _finalValue(baseValue), _statName(statName) {}

ModifiableStat::ModifiableStat(const ModifiableStat &other)
    : _statName(other._statName), isDirty(other.isDirty),
      _baseValue(other._baseValue), _finalValue(other._finalValue),
      _flatMods(other._flatMods), _addMods(other._addMods),
      _mulMods(other._mulMods) {}

ModifiableStat::ModifiableStat(ModifiableStat &&other) noexcept
    : _statName(std::move(other._statName)), isDirty(other.isDirty),
      _baseValue(other._baseValue), _finalValue(other._finalValue),
      _flatMods(std::move(other._flatMods)),
      _addMods(std::move(other._addMods)), _mulMods(std::move(other._mulMods)),
      OnValueChanged(std::move(other.OnValueChanged)) {}

ModifiableStat &ModifiableStat::operator=(const ModifiableStat &other) {
  if (this != &other) {
    _statName = other._statName;
    isDirty = other.isDirty;
    _baseValue = other._baseValue;
    _finalValue = other._finalValue;
    _flatMods = other._flatMods;
    _addMods = other._addMods;
    _mulMods = other._mulMods;
    OnValueChanged.Invoke(GetValue());
  }
  return *this;
}

ModifiableStat &ModifiableStat::operator=(ModifiableStat &&other) noexcept {
  if (this != &other) {
    _statName = std::move(other._statName);
    isDirty = other.isDirty;
    _baseValue = other._baseValue;
    _finalValue = other._finalValue;
    _flatMods = std::move(other._flatMods);
    _addMods = std::move(other._addMods);
    _mulMods = std::move(other._mulMods);
    OnValueChanged = std::move(other.OnValueChanged);
    OnValueChanged.Invoke(GetValue());
  }
  return *this;
}

void ModifiableStat::AddOrUpdateModifier(std::vector<StatModifier> &mods,
                                         const StatModifier mod) {
  auto it =
      find_if(mods.begin(), mods.end(), [&](const StatModifier &existingMod) {
        return existingMod.GetSourceId() == mod.GetSourceId();
      });
  if (it != mods.end()) // 갱신
  {
    it->SetStackedCount(it->GetStackedCount() + 1); // 스택 쌓기
    isDirty = true;
    return;
  } else {
    mods.push_back(mod);
    isDirty = true;
  }
}
void ModifiableStat::AddStatModifier(StatModifier modifier) {
  switch (modifier.GetCalcType()) {
  case StatModifierType::Flat:
    AddOrUpdateModifier(_flatMods, modifier);
    break;
  case StatModifierType::Addition:
    AddOrUpdateModifier(_addMods, modifier);
    break;
  case StatModifierType::Multiplication:
    AddOrUpdateModifier(_mulMods, modifier);
    break;
  }
  OnValueChanged.Invoke(GetValue());
}
void ModifiableStat::RemoveStatModifier(uint32_t sourceId) {
  auto removeCond = [&](const StatModifier &mod) {
    return mod.GetSourceId() == sourceId;
  };
  _flatMods.erase(remove_if(_flatMods.begin(), _flatMods.end(), removeCond),
                  _flatMods.end());
  _addMods.erase(remove_if(_addMods.begin(), _addMods.end(), removeCond),
                 _addMods.end());
  _mulMods.erase(remove_if(_mulMods.begin(), _mulMods.end(), removeCond),
                 _mulMods.end());
  isDirty = true;
  OnValueChanged.Invoke(GetValue());
}
string ModifiableStat::ToString() const {
  ostringstream oss;
  oss << GetStatName() << ": " << GetBaseValue() << " => " << GetValue();
  return oss.str();
}
int ModifiableStat::GetValue() const {
  if (!isDirty)
    return _finalValue;
  float finalCalc = static_cast<float>(_baseValue);
  for (const auto &mod : _flatMods) {
    finalCalc += mod.GetValue() * mod.GetStackedCount();
  }
  float sumPercent = 0.0f; // 합연산용
  for (const auto &mod : _addMods) {
    sumPercent += mod.GetValue() * mod.GetStackedCount();
  }
  finalCalc *= (1 + (sumPercent / 100.0f));
  for (const auto &mod : _mulMods) {
    for (int i = 0; i < mod.GetStackedCount(); i++) {
      finalCalc *= (1 + (mod.GetValue() / 100.0f));
    }
  }
  _finalValue = static_cast<int>(finalCalc);
  isDirty = false;
  return _finalValue;
}

} // namespace TTOT::Stats