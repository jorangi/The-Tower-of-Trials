#pragma once
#include "Class/IClass.h"
#include "Core/ISaveData.h"
#include "Utility/Action.h"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdint.h>
#include <string>
#include <vector>

namespace TTOT::Class {
class ClassBase : public IClass, public Core::ISaveData {
private:
  // 직업 기본 정보
  uint32_t classId;
  std::string className;
  std::string classDesc;
  // 기본치
  float hp;
  float mp;
  float str;
  float dex;
  float _int;
  float wis;
  float cha;
  float def;
  float spd;
  // 보정치
  float hpMultiplier;
  float mpMultiplier;
  float strMultiplier;
  float dexMultiplier;
  float intMultiplier;
  float wisMultiplier;
  float chaMultiplier;
  float defMultiplier;
  float spdMultiplier;
  // 직업 내장 스킬
  std::vector<uint32_t> skills;

  TTOT::Utilities::Action<uint32_t> OnClassIdChanged;
  TTOT::Utilities::Action<std::string> OnClassNameChanged;
  TTOT::Utilities::Action<std::string> OnClassDescChanged;
  TTOT::Utilities::Action<float> OnHpChanged;
  TTOT::Utilities::Action<float> OnMpChanged;
  TTOT::Utilities::Action<float> OnStrChanged;
  TTOT::Utilities::Action<float> OnDexChanged;
  TTOT::Utilities::Action<float> OnIntChanged;
  TTOT::Utilities::Action<float> OnWisChanged;
  TTOT::Utilities::Action<float> OnChaChanged;
  TTOT::Utilities::Action<float> OnDefChanged;
  TTOT::Utilities::Action<float> OnSpdChanged;
  TTOT::Utilities::Action<float> OnHpMultiplierChanged;
  TTOT::Utilities::Action<float> OnMpMultiplierChanged;
  TTOT::Utilities::Action<float> OnStrMultiplierChanged;
  TTOT::Utilities::Action<float> OnDexMultiplierChanged;
  TTOT::Utilities::Action<float> OnIntMultiplierChanged;
  TTOT::Utilities::Action<float> OnWisMultiplierChanged;
  TTOT::Utilities::Action<float> OnChaMultiplierChanged;
  TTOT::Utilities::Action<float> OnDefMultiplierChanged;
  TTOT::Utilities::Action<float> OnSpdMultiplierChanged;
  TTOT::Utilities::Action<std::vector<uint32_t>> OnSkillsChanged;

public:
  virtual std::string ToString() const override {
    return className + " : " + classDesc;
  }
  std::string GetName() const { return className; };
  std::string GetDesc() const { return classDesc; };
  float GetHp() const { return hp; };
  float GetMp() const { return mp; };
  float GetStr() const { return str; };
  float GetDex() const { return dex; };
  float GetInt() const { return _int; };
  float GetWis() const { return wis; };
  float GetCha() const { return cha; };
  float GetDef() const { return def; };
  float GetSpd() const { return spd; };
  float GetHpMultiplier() const { return hpMultiplier; };
  float GetMpMultiplier() const { return mpMultiplier; };
  float GetStrMultiplier() const { return strMultiplier; };
  float GetDexMultiplier() const { return dexMultiplier; };
  float GetIntMultiplier() const { return intMultiplier; };
  float GetWisMultiplier() const { return wisMultiplier; };
  float GetChaMultiplier() const { return chaMultiplier; };
  float GetDefMultiplier() const { return defMultiplier; };
  float GetSpdMultiplier() const { return spdMultiplier; };
  std::vector<uint32_t> GetSkills() const { return skills; };

  TTOT::Utilities::Action<uint32_t> &GetClassIdChanged() {
    return OnClassIdChanged;
  }
  TTOT::Utilities::Action<std::string> &GetClassNameChanged() {
    return OnClassNameChanged;
  }
  TTOT::Utilities::Action<std::string> &GetClassDescChanged() {
    return OnClassDescChanged;
  }
  TTOT::Utilities::Action<float> &GetHpChanged() { return OnHpChanged; }
  TTOT::Utilities::Action<float> &GetMpChanged() { return OnMpChanged; }
  TTOT::Utilities::Action<float> &GetStrChanged() { return OnStrChanged; }
  TTOT::Utilities::Action<float> &GetDexChanged() { return OnDexChanged; }
  TTOT::Utilities::Action<float> &GetIntChanged() { return OnIntChanged; }
  TTOT::Utilities::Action<float> &GetWisChanged() { return OnWisChanged; }
  TTOT::Utilities::Action<float> &GetChaChanged() { return OnChaChanged; }
  TTOT::Utilities::Action<float> &GetDefChanged() { return OnDefChanged; }
  TTOT::Utilities::Action<float> &GetSpdChanged() { return OnSpdChanged; }
  TTOT::Utilities::Action<float> &GetHpMultiplierChanged() {
    return OnHpMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetMpMultiplierChanged() {
    return OnMpMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetStrMultiplierChanged() {
    return OnStrMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetDexMultiplierChanged() {
    return OnDexMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetIntMultiplierChanged() {
    return OnIntMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetWisMultiplierChanged() {
    return OnWisMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetChaMultiplierChanged() {
    return OnChaMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetDefMultiplierChanged() {
    return OnDefMultiplierChanged;
  }
  TTOT::Utilities::Action<float> &GetSpdMultiplierChanged() {
    return OnSpdMultiplierChanged;
  }
  TTOT::Utilities::Action<std::vector<uint32_t>> &GetSkillsChanged() {
    return OnSkillsChanged;
  }

  void Serialize(nlohmann::json &j) const override {
    j = nlohmann::json{{"classId", classId},
                       {"className", className},
                       {"classDesc", classDesc},
                       {"hp", hp},
                       {"mp", mp},
                       {"str", str},
                       {"dex", dex},
                       {"int", _int},
                       {"wis", wis},
                       {"cha", cha},
                       {"def", def},
                       {"spd", spd},
                       {"hpMultiplier", hpMultiplier},
                       {"mpMultiplier", mpMultiplier},
                       {"strMultiplier", strMultiplier},
                       {"dexMultiplier", dexMultiplier},
                       {"intMultiplier", intMultiplier},
                       {"wisMultiplier", wisMultiplier},
                       {"chaMultiplier", chaMultiplier},
                       {"defMultiplier", defMultiplier},
                       {"spdMultiplier", spdMultiplier},
                       {"skills", skills}};
  }
  void Deserialize(const nlohmann::json &j) override {
    j.at("className").get_to(className);
    OnClassNameChanged.Invoke(className);

    j.at("classDesc").get_to(classDesc);
    OnClassDescChanged.Invoke(classDesc);

    j.at("hp").get_to(hp);
    OnHpChanged.Invoke(hp);

    j.at("mp").get_to(mp);
    OnMpChanged.Invoke(mp);

    j.at("str").get_to(str);
    OnStrChanged.Invoke(str);

    j.at("dex").get_to(dex);
    OnDexChanged.Invoke(dex);

    j.at("int").get_to(_int);
    OnIntChanged.Invoke(_int);

    j.at("wis").get_to(wis);
    OnWisChanged.Invoke(wis);

    j.at("cha").get_to(cha);
    OnChaChanged.Invoke(cha);

    j.at("def").get_to(def);
    OnDefChanged.Invoke(def);

    j.at("spd").get_to(spd);
    OnSpdChanged.Invoke(spd);

    j.at("hpMultiplier").get_to(hpMultiplier);
    OnHpMultiplierChanged.Invoke(hpMultiplier);

    j.at("mpMultiplier").get_to(mpMultiplier);
    OnMpMultiplierChanged.Invoke(mpMultiplier);

    j.at("strMultiplier").get_to(strMultiplier);
    OnStrMultiplierChanged.Invoke(strMultiplier);

    j.at("dexMultiplier").get_to(dexMultiplier);
    OnDexMultiplierChanged.Invoke(dexMultiplier);

    j.at("intMultiplier").get_to(intMultiplier);
    OnIntMultiplierChanged.Invoke(intMultiplier);

    j.at("wisMultiplier").get_to(wisMultiplier);
    OnWisMultiplierChanged.Invoke(wisMultiplier);

    j.at("chaMultiplier").get_to(chaMultiplier);
    OnChaMultiplierChanged.Invoke(chaMultiplier);

    j.at("defMultiplier").get_to(defMultiplier);
    OnDefMultiplierChanged.Invoke(defMultiplier);

    j.at("spdMultiplier").get_to(spdMultiplier);
    OnSpdMultiplierChanged.Invoke(spdMultiplier);

    if (j.contains("classId")) {
      j.at("classId").get_to(classId);
      OnClassIdChanged.Invoke(classId);
    }
    if (j.contains("skills")) {
      j.at("skills").get_to(skills);
      OnSkillsChanged.Invoke(skills);
    }
  }
};
} // namespace TTOT::Class