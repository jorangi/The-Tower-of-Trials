#pragma once
#include <functional>
#include "Entity/Player.h"

namespace TTOT::Core::Events
{
    struct PlayerGetEvent
    {
        std::function<void(const TTOT::Entities::Player&)> onLoaded;
    };
}