#pragma once
#include <string>

namespace TTOT::Class
{
    class IClass
    {
        public:
            virtual ~IClass() = default;
            virtual std::string ToString() const = 0;
    };
}