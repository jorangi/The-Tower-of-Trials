#pragma once

namespace TTOT::Core::Events
{
    enum class VolumeType
    {
        Master,
        BGM,
        SFX
    };
    struct VolumeChangeEvent
    {
        VolumeType type;
        int value;
    };
}