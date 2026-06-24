#pragma once
#include "Stat/StatModifier.h"
#include <stdint.h>

namespace TTOT::Core::Events {
enum class StatType { HP, MP, Str, Dex, Int, Wis, Cha, Def, Spd };
struct AddStatModifiableEvent {
  uint32_t entityId;
  StatType statType;
  TTOT::Stats::StatModifier mod;
};
} // namespace TTOT::Core::Events