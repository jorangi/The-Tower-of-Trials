#include <string>

namespace TTOT::Core::Events
{
    struct SaveCompletevent
    {
        int slotIndex;
        bool success;
        std::string errorMessage;
    };
}