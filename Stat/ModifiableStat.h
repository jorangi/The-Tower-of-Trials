#pragma once
#include <string>
#include <vector>
#include "StatModifier.h"

namespace TTOT::Stats
{
    class ModifiableStat
    {
        private:
            std::string _statName; // 스탯 이름
            mutable bool isDirty = false; // 변경 여부를 위한 더티 플래그, const 함수를 위한 예외 처리
            int _baseValue; // 기본 값
            mutable int _finalValue; //계산 값, const 함수를 위한 예외 처리
            std::vector<StatModifier> _flatMods; // 단순 합 연산용
            std::vector<StatModifier> _addMods; // 합연산용
            std::vector<StatModifier> _mulMods; // 곱연산용
            
            void AddOrUpdateModifier(std::vector<StatModifier>& mods, const StatModifier mod);

        public:
            ModifiableStat(int baseValue = 0, std::string statName = "");
            void AddStatModifier(StatModifier modifier);
            void RemoveStatModifier(uint32_t sourceId);
            std::string ToString() const;
            int GetValue() const;
            const std::string GetStatName() const {return _statName;}
            const int GetBaseValue() const {return _baseValue;}
    };
}