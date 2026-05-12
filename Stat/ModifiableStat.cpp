#include <iostream>
#include <algorithm>
#include <math.h>
#include "ModifiableStat.h"
#include "StatModifier.h"

using namespace std;

namespace TTOT::Stats
{    
    ModifiableStat::ModifiableStat(int baseValue, string statName):_baseValue(baseValue), _finalValue(baseValue), _statName(statName) {}
    void ModifiableStat::AddOrUpdateModifier(std::vector<StatModifier>& mods, const StatModifier mod)
    {
        auto it = find_if(mods.begin(), mods.end(), 
            [&](const StatModifier& existingMod)
            {
                return existingMod.GetSourceId() == mod.GetSourceId();
            });
        if(it != mods.end()) // 갱신
        {
            it->SetStackedCount(it->GetStackedCount() + 1); //스택 쌓기
            isDirty = true;
            return;
        }
        else
        {
            mods.push_back(mod);
            isDirty = true;
        }
    }
    void ModifiableStat::AddStatModifier(StatModifier modifier)
    {
        switch(modifier.GetCalcType())
        {
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
    }
    void ModifiableStat::RemoveStatModifier(uint32_t sourceId)
    {
        auto removeCond = [&](const StatModifier& mod)
        {
            return mod.GetSourceId() == sourceId;
        };
        _flatMods.erase(
            remove_if(_flatMods.begin(), _flatMods.end(), removeCond),
            _flatMods.end()
        );
        _addMods.erase(
            remove_if(_addMods.begin(), _addMods.end(), removeCond),
            _addMods.end()
        );
        _mulMods.erase(
            remove_if(_mulMods.begin(), _mulMods.end(), removeCond),
            _mulMods.end()
        );
        isDirty = true;
    }
    string ModifiableStat::ToString() const
    {
        return "";
    }
    int ModifiableStat::GetValue() const
    {
        if(!isDirty) return _finalValue;
        float finalCalc = static_cast<float>(_baseValue);
        for(const auto& mod : _flatMods)
        {
            finalCalc += mod.GetValue() * mod.GetStackedCount();
        }
        float sumPercent = 0.0f; //합연산용
        for(const auto& mod : _addMods)
        {
            sumPercent += mod.GetValue() * mod.GetStackedCount();
        }
        finalCalc *= (1 + (sumPercent / 100.0f));
        for(const auto& mod : _mulMods)
        {
            for(int i = 0; i < mod.GetStackedCount(); i++)
            {
                finalCalc *= (1 + (mod.GetValue() / 100.0f));
            }
        }
        _finalValue = static_cast<int>(finalCalc);
        isDirty = false;
        return _finalValue;
    }
}