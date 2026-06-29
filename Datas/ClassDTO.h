#pragma once
#include <string>
#include <vector>
#include <stdint.h>
#include <nlohmann/json.hpp>

namespace TTOT::Datas {

class ClassDTO {
private:
  uint32_t classId;
  std::string className;
  std::string classDesc;
  float hp;
  float mp;
  float str;
  float dex;
  float _int;
  float wis;
  float cha;
  float def;
  float spd;
  float hpMultiplier;
  float mpMultiplier;
  float strMultiplier;
  float dexMultiplier;
  float intMultiplier;
  float wisMultiplier;
  float chaMultiplier;
  float defMultiplier;
  float spdMultiplier;
  std::vector<uint32_t> skills;

public:
  ClassDTO()
      : classId(0), hp(0.0f), mp(0.0f), str(0.0f), dex(0.0f), _int(0.0f),
        wis(0.0f), cha(0.0f), def(0.0f), spd(0.0f), hpMultiplier(1.0f),
        mpMultiplier(1.0f), strMultiplier(1.0f), dexMultiplier(1.0f),
        intMultiplier(1.0f), wisMultiplier(1.0f), chaMultiplier(1.0f),
        defMultiplier(1.0f), spdMultiplier(1.0f) {}

  ClassDTO(uint32_t classId, const std::string &className, const std::string &classDesc,
           float hp, float mp, float str, float dex, float _int, float wis, float cha,
           float def, float spd, float hpMultiplier, float mpMultiplier,
           float strMultiplier, float dexMultiplier, float intMultiplier,
           float wisMultiplier, float chaMultiplier, float defMultiplier,
           float spdMultiplier, std::vector<uint32_t> skills)
      : classId(classId), className(className), classDesc(classDesc), hp(hp), mp(mp),
        str(str), dex(dex), _int(_int), wis(wis), cha(cha), def(def), spd(spd),
        hpMultiplier(hpMultiplier), mpMultiplier(mpMultiplier),
        strMultiplier(strMultiplier), dexMultiplier(dexMultiplier),
        intMultiplier(intMultiplier), wisMultiplier(wisMultiplier),
        chaMultiplier(chaMultiplier), defMultiplier(defMultiplier),
        spdMultiplier(spdMultiplier), skills(skills) {}

  uint32_t GetClassId() const { return classId; }
  std::string GetClassName() const { return className; }
  std::string GetClassDesc() const { return classDesc; }
  float GetHp() const { return hp; }
  float GetMp() const { return mp; }
  float GetStr() const { return str; }
  float GetDex() const { return dex; }
  float GetInt() const { return _int; }
  float GetWis() const { return wis; }
  float GetCha() const { return cha; }
  float GetDef() const { return def; }
  float GetSpd() const { return spd; }
  float GetHpMultiplier() const { return hpMultiplier; }
  float GetMpMultiplier() const { return mpMultiplier; }
  float GetStrMultiplier() const { return strMultiplier; }
  float GetDexMultiplier() const { return dexMultiplier; }
  float GetIntMultiplier() const { return intMultiplier; }
  float GetWisMultiplier() const { return wisMultiplier; }
  float GetChaMultiplier() const { return chaMultiplier; }
  float GetDefMultiplier() const { return defMultiplier; }
  float GetSpdMultiplier() const { return spdMultiplier; }
  std::vector<uint32_t> GetSkills() const { return skills; }

  friend void to_json(nlohmann::json& j, const ClassDTO& c) {
      j = nlohmann::json{
          {"classId", c.classId},
          {"className", c.className},
          {"classDesc", c.classDesc},
          {"hp", c.hp},
          {"mp", c.mp},
          {"str", c.str},
          {"dex", c.dex},
          {"int", c._int},
          {"wis", c.wis},
          {"cha", c.cha},
          {"def", c.def},
          {"spd", c.spd},
          {"hpMultiplier", c.hpMultiplier},
          {"mpMultiplier", c.mpMultiplier},
          {"strMultiplier", c.strMultiplier},
          {"dexMultiplier", c.dexMultiplier},
          {"intMultiplier", c.intMultiplier},
          {"wisMultiplier", c.wisMultiplier},
          {"chaMultiplier", c.chaMultiplier},
          {"defMultiplier", c.defMultiplier},
          {"spdMultiplier", c.spdMultiplier},
          {"skills", c.skills}
      };
  }

  friend void from_json(const nlohmann::json& j, ClassDTO& c) {
      j.at("className").get_to(c.className);
      j.at("classDesc").get_to(c.classDesc);
      j.at("hp").get_to(c.hp);
      j.at("mp").get_to(c.mp);
      j.at("str").get_to(c.str);
      j.at("dex").get_to(c.dex);
      j.at("int").get_to(c._int);
      j.at("wis").get_to(c.wis);
      j.at("cha").get_to(c.cha);
      j.at("def").get_to(c.def);
      j.at("spd").get_to(c.spd);
      j.at("hpMultiplier").get_to(c.hpMultiplier);
      j.at("mpMultiplier").get_to(c.mpMultiplier);
      j.at("strMultiplier").get_to(c.strMultiplier);
      j.at("dexMultiplier").get_to(c.dexMultiplier);
      j.at("intMultiplier").get_to(c.intMultiplier);
      j.at("wisMultiplier").get_to(c.wisMultiplier);
      j.at("chaMultiplier").get_to(c.chaMultiplier);
      j.at("defMultiplier").get_to(c.defMultiplier);
      j.at("spdMultiplier").get_to(c.spdMultiplier);
      if (j.contains("classId")) j.at("classId").get_to(c.classId);
      if (j.contains("skills")) j.at("skills").get_to(c.skills);
  }
};

class ClassDTOBuilder {
private:
  uint32_t classId = 0;
  std::string className;
  std::string classDesc;
  float hp = 0.0f;
  float mp = 0.0f;
  float str = 0.0f;
  float dex = 0.0f;
  float _int = 0.0f;
  float wis = 0.0f;
  float cha = 0.0f;
  float def = 0.0f;
  float spd = 0.0f;
  float hpMultiplier = 1.0f;
  float mpMultiplier = 1.0f;
  float strMultiplier = 1.0f;
  float dexMultiplier = 1.0f;
  float intMultiplier = 1.0f;
  float wisMultiplier = 1.0f;
  float chaMultiplier = 1.0f;
  float defMultiplier = 1.0f;
  float spdMultiplier = 1.0f;
  std::vector<uint32_t> skills;

public:
  ClassDTOBuilder() = default;

  ClassDTOBuilder& ClassId(uint32_t classId) { this->classId = classId; return *this; }
  ClassDTOBuilder& ClassName(const std::string& className) { this->className = className; return *this; }
  ClassDTOBuilder& ClassDesc(const std::string& classDesc) { this->classDesc = classDesc; return *this; }
  ClassDTOBuilder& Hp(float hp) { this->hp = hp; return *this; }
  ClassDTOBuilder& Mp(float mp) { this->mp = mp; return *this; }
  ClassDTOBuilder& Str(float str) { this->str = str; return *this; }
  ClassDTOBuilder& Dex(float dex) { this->dex = dex; return *this; }
  ClassDTOBuilder& Int(float _int) { this->_int = _int; return *this; }
  ClassDTOBuilder& Wis(float wis) { this->wis = wis; return *this; }
  ClassDTOBuilder& Cha(float cha) { this->cha = cha; return *this; }
  ClassDTOBuilder& Def(float def) { this->def = def; return *this; }
  ClassDTOBuilder& Spd(float spd) { this->spd = spd; return *this; }
  ClassDTOBuilder& HpMultiplier(float hpMultiplier) { this->hpMultiplier = hpMultiplier; return *this; }
  ClassDTOBuilder& MpMultiplier(float mpMultiplier) { this->mpMultiplier = mpMultiplier; return *this; }
  ClassDTOBuilder& StrMultiplier(float strMultiplier) { this->strMultiplier = strMultiplier; return *this; }
  ClassDTOBuilder& DexMultiplier(float dexMultiplier) { this->dexMultiplier = dexMultiplier; return *this; }
  ClassDTOBuilder& IntMultiplier(float intMultiplier) { this->intMultiplier = intMultiplier; return *this; }
  ClassDTOBuilder& WisMultiplier(float wisMultiplier) { this->wisMultiplier = wisMultiplier; return *this; }
  ClassDTOBuilder& ChaMultiplier(float chaMultiplier) { this->chaMultiplier = chaMultiplier; return *this; }
  ClassDTOBuilder& DefMultiplier(float defMultiplier) { this->defMultiplier = defMultiplier; return *this; }
  ClassDTOBuilder& SpdMultiplier(float spdMultiplier) { this->spdMultiplier = spdMultiplier; return *this; }
  ClassDTOBuilder& Skills(const std::vector<uint32_t>& skills) { this->skills = skills; return *this; }

  ClassDTO Build() {
    return ClassDTO(classId, className, classDesc, hp, mp, str, dex, _int, wis, cha,
                    def, spd, hpMultiplier, mpMultiplier, strMultiplier,
                    dexMultiplier, intMultiplier, wisMultiplier, chaMultiplier,
                    defMultiplier, spdMultiplier, skills);
  }
};

} // namespace TTOT::Datas
