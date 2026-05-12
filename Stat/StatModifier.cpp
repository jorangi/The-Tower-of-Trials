#include <StatModifier.h>
#include <math.h>

using namespace std;

namespace TTOT::Stats
{
    StatModifier::StatModifier(int val, uint32_t sourceId, StatModifierType calcType, int stackedCount, int maxStackedCount)
    : _value(val), _sourceId(sourceId), _calcType(calcType), _stackedCount(stackedCount), _maxStackedCount(maxStackedCount){}
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
        return _stackedCount;
    }
    const int StatModifier::GetMaxStackedCount() const
    {
        return _maxStackedCount;
    }
    void StatModifier::SetStackedCount(int count)
    {
        _stackedCount = min(_maxStackedCount, count); //최대 중첩까지만 가능하게끔, 중첩 불가의 경우 기본값인 1이니 상관 X
    }
    void StatModifier::SetMaxStackedCount(int count)
    {
        _maxStackedCount = max(1, count);
    }
}