#pragma once
#include <stdint.h>

namespace TTOT::Core::Events
{
    struct ChangeHealthEvent
    {
        uint32_t entityId;
        int delta;
    };
}
