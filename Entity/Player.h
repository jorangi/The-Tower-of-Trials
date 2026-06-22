#pragma once
#include "Entity.h"
#include <string>

namespace TTOT::Entities
{
    class Player : public Entity
    {
        private:
            std::string purpose;
            std::string guidance;
        public:
            Player(const uint32_t id, const std::string& name, const bool gender = false): Entity(id, name){}
            std::string ToString() const override;
            void ShowInfo() const;
            void SetPurpose(const std::string& purpose);
            void SetGuidance(const std::string& guidance);
            void Serialize(nlohmann::json& j) const override;
            void Deserialize(const nlohmann::json& j) override;
    };
}