#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "StatModifier.h"

using namespace std;

namespace TTOT::stats
{
    StatModifier::StatModifier(int val, uint32_t sourceId, StatModifierType calcType, int stackedCount, int maxStackedCount)
    : _value(val), _sourceId(sourceId), _calcType(calcType), stackedCount(stackedCount), maxStackedCount(maxStackedCount){}
    const string StatModifier::ToString() const
    {
        
        string typeStr;
        switch(_calcType)
        {
            case StatModifierType::Flat:
                typeStr = "단순 합";
                break;
            case StatModifierType::Addition:
                typeStr = "합연산";
                break;
            case StatModifierType::Multiplication:
                typeStr = "곱연산";
                break;
        }
        ostringstream oss;
        oss << _sourceId << "(으)로 부터 " << _value << "만큼" << typeStr;
        return oss.str();
    }
    const int StatModifier::GetValue() const
    {
        return _value;
    }
    const StatModifierType StatModifier::GetCalcType() const
    {
        return _calcType;
    }
    const uint32_t StatModifier::GetSourceId() const
    {
        return _sourceId;
    }
    const int StatModifier::GetStackedCount() const
    {
        return stackedCount;
    }
    const int StatModifier::GetMaxStackedCount() const
    {
        return maxStackedCount;
    }
    
    class ModifiableStat
    {
        private:
            string _statName; // 스탯 이름
            bool isDirty = false; // 변경 여부를 위한 더티 플래그
            int _baseValue; // 기본 값
            int _finalValue; //계산 값
            vector<StatModifier> _flatMods; // 단순 합 연산용
            vector<StatModifier> _addMods; // 합연산용
            vector<StatModifier> _mulMods; // 곱연산용

            void AddOrUpdateModifier(vector<StatModifier>& mods, const StatModifier mod)
            {
                auto it = find_if(mods.begin(), mods.end(), 
                    [&](const StatModifier& existingMod)
                    {
                        return existingMod.GetSourceId() == mod.GetSourceId();
                    });
                if(it != mods.end()) // 갱신
                {
                    int count = min(it->GetStackedCount() + 1, mod.GetMaxStackedCount()); //최대 중첩까지만 가능하게끔, 중첩 불가의 경우 기본값인 1이니 상관 X
                    it->SetStackedCount(count); //스택 쌓기
                    isDirty = true;
                    return;
                }
                else
                {
                    mods.push_back(mod);
                    isDirty = true;
                }
            }
            
        public:
            const void AddStatModifier(StatModifier modifier)
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
            const void RemoveStatModifier(uint32_t sourceId)
            {
                for(auto it = _flatMods.begin(); it != _flatMods.end(); it++)
                {
                    _flatMods.erase(it);
                }
                for(auto it = _addMods.begin(); it != _addMods.end(); it++)
                {
                    _addMods.erase(it);
                }
                for(auto it = _mulMods.begin(); it != _mulMods.end(); it++)
                {
                    _mulMods.erase(it);
                }
                isDirty = true;
            }
            const string ToString() const
            {
                return "";
            }
            const int GetValue()
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
    };
}