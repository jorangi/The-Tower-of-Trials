#include <stdint.h>
#include <string>
#include <sstream>

using namespace std;

namespace TTOT::stats
{
    enum class StatModifierType
    {
        Flat, //단순 합 ex) 10 + 2 + 3
        Addition, //합연산 ex) 100 + 10% + 10% = 120
        Multiplication, //곱연산 ex) 100 * 10% * 10% = 121
    };
    class StatModifier
    {
        private:
            int _value; //값
            uint32_t _sourceId; //소스 ID
            StatModifierType _calcType = StatModifierType::Flat; //계산 방식
            int stackedCount = 1; //중첩 횟수, 기본 값은 1로 하여 계산 편의
            int maxStackedCount = 1; //최대 중첩 횟수, 기본 값은 1로 하여 계산 편의

        public:
            StatModifier(int val, uint32_t sourceId, StatModifierType calcType = StatModifierType::Flat, int stackedCount = 1, int maxStackedCount = 1);

            const string ToString() const;
            const int GetValue() const;
            const StatModifierType GetCalcType() const;
            const uint32_t GetSourceId() const;
            const int GetStackedCount() const;
            const int GetMaxStackedCount() const;
            void SetStackedCount(int count);
            void SetMaxStackedCount(int count);
    };
}