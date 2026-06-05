#pragma once
#include <functional>

namespace TTOT::Core::Events
{
    struct VolumeGetEvent
    {
        std::function<void(float master, float bgm, float sfx)> onLoaded;
    };
}