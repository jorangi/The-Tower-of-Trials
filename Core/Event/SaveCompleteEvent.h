#include <string>

namespace TTOT::Core::Events
{
    struct SaveCompletevent
    {
        int slotIndex;
        bool success;
        const std::string& errorMessage;
    };
}