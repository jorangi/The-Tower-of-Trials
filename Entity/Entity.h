#pragma once
#include "Component/Components.h"
#include "Core/ISaveData.h"
#include "Utility/Action.h"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTOT::Class {
class ClassBase;
}

namespace TTOT::Entities {
class Entity : public Core::ISaveData {
private:
  struct ComponentNode {
    uint32_t id;
    uint32_t typeID;
    std::unique_ptr<Components::Component> component;
  };
  Utilities::Action<std::string> OnNameChanged;
  Utilities::Action<bool> OnGenderChanged;

protected:
  uint32_t _id;
  std::string _name;
  bool gender = false; // f: male, t: female
  uint32_t _next_id = 0;
  std::vector<Components::Component *> _compArr;
  std::vector<ComponentNode> _comps;
  std::unique_ptr<TTOT::Class::ClassBase> classInfo;

public:
  Entity(uint32_t id, const std::string &name = "");
  virtual ~Entity();
  virtual void Serialize(nlohmann::json &j) const override;
  virtual void Deserialize(const nlohmann::json &j) override;
  void AddComponent(std::unique_ptr<Components::Component> comp);
  template <typename T> T *GetComponent() const {
    uint32_t typeID = Components::GetComponentTypeID<T>();
    if (typeID >= _compArr.size())
      return nullptr;
    return static_cast<T *>(_compArr[typeID]);
  }
  void RemoveComponent(uint32_t id);
  Utilities::Action<std::string> &GetNameChanged();
  Utilities::Action<bool> &GetGenderChanged();

  virtual uint32_t GetId() const;
  virtual std::string GetName() const;
  const bool GetGender() const;
};
} // namespace TTOT::Entities