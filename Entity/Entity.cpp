#include "Entity.h"

namespace TTOT::Entities
{ 
    Entity::Entity(uint32_t id, const std::string& name)
        : _id(id), _name(name.empty() ? std::to_string(id) : name) {}
}