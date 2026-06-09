#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <memory>
#include <algorithm>
#include "Component/Components.h"
#include "Class/ClassBase.h"

namespace TTOT::Entities
{
    class Entity
    {
        private:
            struct ComponentNode
            {
                uint32_t id;
                uint32_t typeID;
                std::unique_ptr<Components::Component> component;
            };
        protected:
            uint32_t _id;
            std::string _name;
            bool gender; //f: male, t: female
            uint32_t _next_id = 0;
            std::vector<Components::Component*> _compArr;
            std::vector<ComponentNode> _comps;
            std::unique_ptr<TTOT::Class::ClassBase> classInfo;
        public:
            Entity(uint32_t id, const std::string& name="");
            virtual ~Entity() = default;
            void AddComponent(std::unique_ptr<Components::Component> comp)
            {
                if(!comp) return;
                uint32_t comp_id = _next_id++;
                uint32_t typeID = comp->GetTypeID();
                if(typeID >= _compArr.size())
                {
                    _compArr.resize(typeID + 1, nullptr);
                }
                _compArr[typeID] = comp.get();
                _comps.push_back({comp_id, typeID, std::move(comp)});
            }
            template<typename T>
            T* GetComponent() const
            {
                uint32_t typeID = Components::GetComponentTypeID<T>();
                if(typeID >= _compArr.size()) return nullptr;
                return static_cast<T*>(_compArr[typeID]);
            }
            void RemoveComponent(uint32_t id)
            {
                auto it = std::find_if(_comps.begin(), _comps.end(), [id](const ComponentNode& node)
                {
                    return node.id==id;
                });
                if(it != _comps.end())
                {
                    uint32_t typeID = it->typeID;
                    if(typeID < _compArr.size())
                    {
                        _compArr[typeID] = nullptr;
                    }
                    _comps.erase(it);
                }
            }
            virtual uint32_t GetId() const {return _id;}
            virtual std::string GetName() const {return _name;}
            const bool GetGender() const {return gender;}
            virtual std::string ToString() const = 0;
    };
}