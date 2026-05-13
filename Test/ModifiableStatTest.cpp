#include <iostream>
#include <windows.h>
#include <cassert>
#include "Stat/ModifiableStat.h"

using namespace std;
using namespace TTOT::Stats;

StatModifier Create_StatModifier(int val, uint32_t sourceId, StatModifierType type, int stackedCount = 1, int maxStackedCount = 1)
{
    StatModifier mod(val, sourceId, type, stackedCount, maxStackedCount);
    //StatModifierTest에서 확인함
    return mod;
};

ModifiableStat Creation_Test(int baseValue, string statName)
{
    ModifiableStat stat(baseValue, statName);
    assert(stat.GetBaseValue() == baseValue);
    assert(stat.GetStatName() == statName);
    cout << "ModifiableStat 생성 테스트 통과" << endl << stat.ToString() << endl;
    return stat;
}
void AddModifierTest(ModifiableStat& stat, StatModifier mod, int expectedValue)
{
    stat.AddStatModifier(mod);
    assert(stat.GetValue() == expectedValue);
    cout << "StatModifier " << mod.ToString() << "추가 테스트 통과" << endl << stat.ToString() << endl;
}
void RemoveModifierTest(ModifiableStat& stat, uint32_t sourceId, int expectedValue)
{
    stat.RemoveStatModifier(sourceId);
    assert(stat.GetValue() == expectedValue);
    cout << "StatModifier sourceId " << sourceId << "삭제 테스트 통과" << endl << stat.ToString() << endl;
}
int main()
{
    ModifiableStat stat = Creation_Test(100, "HP");
    StatModifier mod1 = Create_StatModifier(10, 1, StatModifierType::Flat);
    AddModifierTest(stat, mod1, 110);
    StatModifier mod2 = Create_StatModifier(10, 1, StatModifierType::Addition, 1, 2);
    AddModifierTest(stat, mod2, 121);
    StatModifier mod3 = Create_StatModifier(10, 2, StatModifierType::Multiplication, 4, 2);
    AddModifierTest(stat, mod3, 146);
    RemoveModifierTest(stat, 1, 121);
}