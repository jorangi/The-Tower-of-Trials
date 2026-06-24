#pragma once
#include <stdint.h>

namespace TTOT::Core::Events
{
    struct ChangeManaEvent
    {
        uint32_t entityId;
        int delta;
    };
}
