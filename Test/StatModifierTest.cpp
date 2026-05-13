#include <iostream>
#include <assert.h>
#include "Stat/StatModifier.h"
#include <windows.h>

using namespace std;
using TTOT::Stats::StatModifier;
using TTOT::Stats::StatModifierType;

void Test_Creation(int val, uint32_t sourceId, StatModifierType type, int stackedCount = 1, int maxStackedCount = 1)
{
    StatModifier mod(val, sourceId, type, stackedCount, maxStackedCount);
    assert(mod.GetValue() == val);
    assert(mod.GetSourceId() == sourceId);
    assert(mod.GetCalcType() == type);
    assert(mod.GetStackedCount() == stackedCount);
    assert(mod.GetMaxStackedCount() == maxStackedCount);

    cout << "생성 테스트 통과" << endl;
};

// int main(void)
// {
//     SetConsoleOutputCP(CP_UTF8);
//     Test_Creation(10, 1, StatModifierType::Flat);
//     Test_Creation(10, 1, StatModifierType::Addition, 1, 2);
//     Test_Creation(10, 1, StatModifierType::Multiplication, 4, 2);
// }