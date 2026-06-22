#pragma once
#include <nlohmann/json_fwd.hpp>
#include <string>
namespace TTOT::Core
{
    struct ISaveData
    {
        virtual ~ISaveData() = default;
        virtual std::string ToString() const = 0;
        virtual void Serialize(nlohmann::json& j) const = 0;
        virtual void Deserialize(const nlohmann::json& j) = 0;
    };
    inline void to_json(nlohmann::json& j, const ISaveData& obj)
    {
        obj.Serialize(j);
    }
    inline void from_json(const nlohmann::json& j, ISaveData& obj)
    {
        obj.Deserialize(j);
    }
}