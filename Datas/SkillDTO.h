#pragma once
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTOT::Datas {

enum class SkillType { Active, Passive };
enum class TargetType { Self, SingleEnemy, AllEnemies, SingleAlly, AllAllies };
enum class ScalingStat { None, HP, MP, STR, DEX, INT, WIS, CHA, DEF, SPD };
enum class StatusEffectType { None, Poison, Stun, Burn, Freeze, Bleed };
enum class PassiveTriggerType { None, Constant, OnAttack, OnDefend, OnLowHP, OnKill };

class SkillDTO {
private:
  uint32_t skillId;
  std::string name;
  std::string desc;
  SkillType skillType;
  TargetType targetType;
  uint32_t manaCost;
  uint32_t cooldown;

  // Combat Effect Details (Active)
  int baseDamage;
  int baseHeal;
  ScalingStat scalingStat;
  float scalingMultiplier;

  // Status Effect Details (Active)
  StatusEffectType statusEffect;
  float effectChance; // 0.0f to 1.0f
  int effectDuration; // in turns

  // Passive Effect Details
  PassiveTriggerType passiveTrigger;
  ScalingStat passiveBuffStat;
  int passiveBuffValue;
  float passiveBuffMultiplier;

public:
  SkillDTO()
      : skillId(0), skillType(SkillType::Active),
        targetType(TargetType::SingleEnemy), manaCost(0), cooldown(0),
        baseDamage(0), baseHeal(0), scalingStat(ScalingStat::None),
        scalingMultiplier(0.0f), statusEffect(StatusEffectType::None),
        effectChance(0.0f), effectDuration(0),
        passiveTrigger(PassiveTriggerType::None),
        passiveBuffStat(ScalingStat::None), passiveBuffValue(0),
        passiveBuffMultiplier(0.0f) {}

  SkillDTO(uint32_t skillId, const std::string &name, const std::string &desc,
           SkillType skillType, TargetType targetType, uint32_t manaCost,
           uint32_t cooldown, int baseDamage, int baseHeal,
           ScalingStat scalingStat, float scalingMultiplier,
           StatusEffectType statusEffect, float effectChance,
           int effectDuration, PassiveTriggerType passiveTrigger,
           ScalingStat passiveBuffStat, int passiveBuffValue,
           float passiveBuffMultiplier)
      : skillId(skillId), name(name), desc(desc), skillType(skillType),
        targetType(targetType), manaCost(manaCost), cooldown(cooldown),
        baseDamage(baseDamage), baseHeal(baseHeal), scalingStat(scalingStat),
        scalingMultiplier(scalingMultiplier), statusEffect(statusEffect),
        effectChance(effectChance), effectDuration(effectDuration),
        passiveTrigger(passiveTrigger), passiveBuffStat(passiveBuffStat),
        passiveBuffValue(passiveBuffValue),
        passiveBuffMultiplier(passiveBuffMultiplier) {}

  uint32_t GetSkillId() const { return skillId; }
  std::string GetName() const { return name; }
  std::string GetDesc() const { return desc; }
  SkillType GetSkillType() const { return skillType; }
  TargetType GetTargetType() const { return targetType; }
  uint32_t GetManaCost() const { return manaCost; }
  uint32_t GetCooldown() const { return cooldown; }

  int GetBaseDamage() const { return baseDamage; }
  int GetBaseHeal() const { return baseHeal; }
  ScalingStat GetScalingStat() const { return scalingStat; }
  float GetScalingMultiplier() const { return scalingMultiplier; }

  StatusEffectType GetStatusEffect() const { return statusEffect; }
  float GetEffectChance() const { return effectChance; }
  int GetEffectDuration() const { return effectDuration; }

  // Passive Getters
  PassiveTriggerType GetPassiveTrigger() const { return passiveTrigger; }
  ScalingStat GetPassiveBuffStat() const { return passiveBuffStat; }
  int GetPassiveBuffValue() const { return passiveBuffValue; }
  float GetPassiveBuffMultiplier() const { return passiveBuffMultiplier; }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SkillDTO, skillId, name, desc, skillType,
                                 targetType, manaCost, cooldown, baseDamage,
                                 baseHeal, scalingStat, scalingMultiplier,
                                 statusEffect, effectChance, effectDuration,
                                 passiveTrigger, passiveBuffStat,
                                 passiveBuffValue, passiveBuffMultiplier)
};

class SkillDTOBuilder {
private:
  uint32_t skillId = 0;
  std::string name;
  std::string desc;
  SkillType skillType = SkillType::Active;
  TargetType targetType = TargetType::SingleEnemy;
  uint32_t manaCost = 0;
  uint32_t cooldown = 0;
  int baseDamage = 0;
  int baseHeal = 0;
  ScalingStat scalingStat = ScalingStat::None;
  float scalingMultiplier = 0.0f;
  StatusEffectType statusEffect = StatusEffectType::None;
  float effectChance = 0.0f;
  int effectDuration = 0;
  PassiveTriggerType passiveTrigger = PassiveTriggerType::None;
  ScalingStat passiveBuffStat = ScalingStat::None;
  int passiveBuffValue = 0;
  float passiveBuffMultiplier = 0.0f;

public:
  SkillDTOBuilder() = default;

  SkillDTOBuilder &SkillId(uint32_t skillId) {
    this->skillId = skillId;
    return *this;
  }
  SkillDTOBuilder &Name(const std::string &name) {
    this->name = name;
    return *this;
  }
  SkillDTOBuilder &Desc(const std::string &desc) {
    this->desc = desc;
    return *this;
  }
  SkillDTOBuilder &Type(SkillType skillType) {
    this->skillType = skillType;
    return *this;
  }
  SkillDTOBuilder &Target(TargetType targetType) {
    this->targetType = targetType;
    return *this;
  }
  SkillDTOBuilder &ManaCost(uint32_t cost) {
    this->manaCost = cost;
    return *this;
  }
  SkillDTOBuilder &Cooldown(uint32_t cooldown) {
    this->cooldown = cooldown;
    return *this;
  }

  // Combat details
  SkillDTOBuilder &Damage(int dmg) {
    this->baseDamage = dmg;
    return *this;
  }
  SkillDTOBuilder &Heal(int heal) {
    this->baseHeal = heal;
    return *this;
  }
  SkillDTOBuilder &Scale(ScalingStat stat, float mult) {
    this->scalingStat = stat;
    this->scalingMultiplier = mult;
    return *this;
  }

  // Status Effects
  SkillDTOBuilder &Debuff(StatusEffectType type, float chance, int duration) {
    this->statusEffect = type;
    this->effectChance = chance;
    this->effectDuration = duration;
    return *this;
  }

  // Passive details
  SkillDTOBuilder &Passive(PassiveTriggerType trigger, ScalingStat stat, int val, float mult) {
    this->passiveTrigger = trigger;
    this->passiveBuffStat = stat;
    this->passiveBuffValue = val;
    this->passiveBuffMultiplier = mult;
    return *this;
  }

  SkillDTO Build() {
    return SkillDTO(skillId, name, desc, skillType, targetType, manaCost,
                    cooldown, baseDamage, baseHeal, scalingStat,
                    scalingMultiplier, statusEffect, effectChance,
                    effectDuration, passiveTrigger, passiveBuffStat,
                    passiveBuffValue, passiveBuffMultiplier);
  }
};

} // namespace TTOT::Datas
