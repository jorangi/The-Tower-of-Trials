#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <memory>
#include <Components.h>

namespace TTOT::Entities
{
    class Entity
    {
        private:
            struct ComponentNode
            {
                uint32_t id;
                std::unique_ptr<Components::Component> component;
            };
        protected:
            uint32_t _id;
            std::string _name;
            uint32_t _next_id = 0;
            std::vector<ComponentNode> _comps;
        public:
            Entity(uint32_t id, const std::string& name="");
            virtual ~Entity() = default;
            void AddComponent(Components::Component* comp)
            {
                uint32_t comp_id = _next_id++;
                _comps.push_back({comp_id, std::unique_ptr<Components::Component>(comp)});
            }
            template<typename T>
            T* GetComponent() const
            {
                for(auto comp : _comps)
                {
                    T* targetComp = dynamic_cast<T*>(comp);
                    if(targetComp != nullptr) return targetComp;
                    return nullptr;
                }
            }
            void RemoveComponent(uint32_t id)
            {
                _comps.erase(
                    std::remove_if(_comps.begin(), _comps.end(), [id](const ComponentNode& node)
                    {
                        return node.id == id;
                    }),
                    _comps.end()
                );
            }
            virtual uint32_t GetId() const {return _id;}
            virtual std::string GetName() const {return _name;}
            virtual std::string ToString() const = 0;
    };
}