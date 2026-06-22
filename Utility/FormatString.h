#include <string>

namespace TTOT::Utilities
{
    inline std::string FormatString(std::string text, const std::string& target, const std::string& value) {
        size_t pos = text.find(target);
        if (pos != std::string::npos) {
            text.replace(pos, target.length(), value);
        }
        return text;
    }
}