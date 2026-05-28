#pragma once
#include "Core/EventBus.h"

namespace TTOT::Core
{
    struct GameContext
    {
        TTOT::Core::EventBus& eventBus;
    };
}