#pragma once
#include <string>
#include <cstdint>

using namespace std;

namespace TTOT::Components
{
    inline uint32_t GetUniqueComponentTypeID()
    {
        static uint32_t lastId = 0;
        return lastId++;
    }
    template<typename T>
    inline uint32_t GetComponentTypeID() noexcept
    {
        static uint32_t typeID = GetUniqueComponentTypeID();
        return typeID;
    }
    class Component
    {
        public:
            virtual ~Component() = default;
            virtual std::string ToString() const = 0;
            virtual uint32_t GetTypeID() const = 0;
    };
}