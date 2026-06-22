#pragma once
#include <string>
namespace TTOT::Core::Events
{
    struct InjectPurposeEvent
    {
        std::string purpose;
        std::string guidance;
    };
}