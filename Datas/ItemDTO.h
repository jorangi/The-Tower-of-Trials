#pragma once
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTOT::Datas {

enum class ItemType { General, Equipment, Consumable };
enum class EquipSlot { None, Weapon, Armor, Accessory };
enum class BuffStatType { None, HP, MP, STR, DEX, INT, WIZ, CHA, DEF, SPD };

class ItemDTO {
private:
  uint32_t itemId;
  std::string name;
  std::string desc;
  ItemType itemType;
  uint32_t price;

  // Equipment Specific
  EquipSlot equipSlot;
  int atkBonus;
  int defBonus;
  int strBonus;
  int dexBonus;
  int intBonus;

  // Consumable Specific
  int healHp;
  int healMp;
  int effectDuration; // 0 for instant, >0 for turns
  BuffStatType buffStat;
  int buffValue;

  // General / Quest Specific
  bool isQuestItem;

public:
  ItemDTO()
      : itemId(0), itemType(ItemType::General), price(0),
        equipSlot(EquipSlot::None), atkBonus(0), defBonus(0), strBonus(0),
        dexBonus(0), intBonus(0), healHp(0), healMp(0), effectDuration(0),
        buffStat(BuffStatType::None), buffValue(0), isQuestItem(false) {}

  ItemDTO(uint32_t itemId, const std::string &name, const std::string &desc,
          ItemType itemType, uint32_t price, EquipSlot equipSlot, int atkBonus,
          int defBonus, int strBonus, int dexBonus, int intBonus, int healHp,
          int healMp, int effectDuration, BuffStatType buffStat, int buffValue,
          bool isQuestItem)
      : itemId(itemId), name(name), desc(desc), itemType(itemType),
        price(price), equipSlot(equipSlot), atkBonus(atkBonus),
        defBonus(defBonus), strBonus(strBonus), dexBonus(dexBonus),
        intBonus(intBonus), healHp(healHp), healMp(healMp),
        effectDuration(effectDuration), buffStat(buffStat),
        buffValue(buffValue), isQuestItem(isQuestItem) {}

  uint32_t GetItemId() const { return itemId; }
  std::string GetName() const { return name; }
  std::string GetDesc() const { return desc; }
  ItemType GetItemType() const { return itemType; }
  uint32_t GetPrice() const { return price; }

  // Equipment Getters
  EquipSlot GetEquipSlot() const { return equipSlot; }
  int GetAtkBonus() const { return atkBonus; }
  int GetDefBonus() const { return defBonus; }
  int GetStrBonus() const { return strBonus; }
  int GetDexBonus() const { return dexBonus; }
  int GetIntBonus() const { return intBonus; }

  // Consumable Getters
  int GetHealHp() const { return healHp; }
  int GetHealMp() const { return healMp; }
  int GetEffectDuration() const { return effectDuration; }
  BuffStatType GetBuffStat() const { return buffStat; }
  int GetBuffValue() const { return buffValue; }

  // General / Quest Getters
  bool IsQuestItem() const { return isQuestItem; }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemDTO, itemId, name, desc, itemType, price,
                                 equipSlot, atkBonus, defBonus, strBonus,
                                 dexBonus, intBonus, healHp, healMp,
                                 effectDuration, buffStat, buffValue,
                                 isQuestItem)
};

class ItemDTOBuilder {
private:
  uint32_t itemId = 0;
  std::string name;
  std::string desc;
  ItemType itemType = ItemType::General;
  uint32_t price = 0;
  EquipSlot equipSlot = EquipSlot::None;
  int atkBonus = 0;
  int defBonus = 0;
  int strBonus = 0;
  int dexBonus = 0;
  int intBonus = 0;
  int healHp = 0;
  int healMp = 0;
  int effectDuration = 0;
  BuffStatType buffStat = BuffStatType::None;
  int buffValue = 0;
  bool isQuestItem = false;

public:
  ItemDTOBuilder() = default;

  ItemDTOBuilder &ItemId(uint32_t itemId) {
    this->itemId = itemId;
    return *this;
  }
  ItemDTOBuilder &Name(const std::string &name) {
    this->name = name;
    return *this;
  }
  ItemDTOBuilder &Desc(const std::string &desc) {
    this->desc = desc;
    return *this;
  }
  ItemDTOBuilder &Type(ItemType itemType) {
    this->itemType = itemType;
    return *this;
  }
  ItemDTOBuilder &Price(uint32_t price) {
    this->price = price;
    return *this;
  }

  // Equipment
  ItemDTOBuilder &Slot(EquipSlot equipSlot) {
    this->equipSlot = equipSlot;
    return *this;
  }
  ItemDTOBuilder &Atk(int atk) {
    this->atkBonus = atk;
    return *this;
  }
  ItemDTOBuilder &Def(int def) {
    this->defBonus = def;
    return *this;
  }
  ItemDTOBuilder &Str(int str) {
    this->strBonus = str;
    return *this;
  }
  ItemDTOBuilder &Dex(int dex) {
    this->dexBonus = dex;
    return *this;
  }
  ItemDTOBuilder &Int(int _int) {
    this->intBonus = _int;
    return *this;
  }

  // Consumables
  ItemDTOBuilder &HealHp(int hp) {
    this->healHp = hp;
    return *this;
  }
  ItemDTOBuilder &HealMp(int mp) {
    this->healMp = mp;
    return *this;
  }
  ItemDTOBuilder &Duration(int duration) {
    this->effectDuration = duration;
    return *this;
  }
  ItemDTOBuilder &Buff(BuffStatType stat, int val) {
    this->buffStat = stat;
    this->buffValue = val;
    return *this;
  }

  // General
  ItemDTOBuilder &Quest(bool isQuest) {
    this->isQuestItem = isQuest;
    return *this;
  }

  ItemDTO Build() {
    return ItemDTO(itemId, name, desc, itemType, price, equipSlot, atkBonus,
                   defBonus, strBonus, dexBonus, intBonus, healHp, healMp,
                   effectDuration, buffStat, buffValue, isQuestItem);
  }
};

} // namespace TTOT::Datas
