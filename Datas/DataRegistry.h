#pragma once

#include <cstdint>
#include <unordered_map>
namespace TTOT::Datas {
enum class DataType { Unknown, Class, Entity, Object, Item, Skill };
class DataRegistry {
private:
  // std::unordered_map<uint32_t, class Ty>
public:
  DataRegistry() = default;
  ~DataRegistry() = default;
  DataType GetDataType(std::uint32_t id) {
    uint32_t type = (id >> 24) & 0xFF;
    switch (type) {
    case 0x01:
      return DataType::Class;
    case 0x02:
      return DataType::Entity;
    case 0x03:
      return DataType::Object;
    case 0x04:
      return DataType::Item;
    case 0x05:
      return DataType::Skill;
    default:
      return DataType::Unknown;
    }
  }
};
} // namespace TTOT::Datas