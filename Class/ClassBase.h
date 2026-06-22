#pragma once
#include "Core/ISaveData.h"
#include "Class/IClass.h"
#include <nlohmann/json_fwd.hpp>
#include <stdint.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace TTOT::Class
{
    class ClassBase : public IClass, public Core::ISaveData
    {
        private:
            //직업 기본 정보
            uint32_t classId;
            std::string className;
            std::string classDesc;
            //기본치
            float hp;
            float mp;
            float str;
            float dex;
            float _int;
            float wis;
            float cha;
            float def;
            float spd;
            //보정치
            float hpMultiplier;
            float mpMultiplier;
            float strMultiplier;
            float dexMultiplier;
            float intMultiplier;
            float wisMultiplier;
            float chaMultiplier;
            float defMultiplier;
            float spdMultiplier;
            //직업 내장 스킬
            std::vector<uint32_t> skills;

        public:
            virtual std::string ToString() const override{return className + " : " + classDesc;}
            std::string GetName() const {return className;};
            std::string GetDesc() const {return classDesc;};
            float GetHp() const {return hp;};
            float GetMp() const {return mp;};
            float GetStr() const {return str;};
            float GetDex() const {return dex;};
            float GetInt() const {return _int;};
            float GetWis() const {return wis;};
            float GetCha() const {return cha;};
            float GetDef() const {return def;};
            float GetSpd() const {return spd;};
            float GetHpMultiplier() const {return hpMultiplier;};
            float GetMpMultiplier() const {return mpMultiplier;};
            float GetStrMultiplier() const {return strMultiplier;};
            float GetDexMultiplier() const {return dexMultiplier;};
            float GetIntMultiplier() const {return intMultiplier;};
            float GetWisMultiplier() const {return wisMultiplier;};
            float GetChaMultiplier() const {return chaMultiplier;};
            float GetDefMultiplier() const {return defMultiplier;};
            float GetSpdMultiplier() const {return spdMultiplier;};
            std::vector<uint32_t> GetSkills() const {return skills;};

            void Serialize(nlohmann::json& j) const override
            {
                j = nlohmann::json
                {
                    {"classId", classId},
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
                        {"skills", skills}
                };
            }
            void Deserialize(const nlohmann::json& j) override
            {
                j.at("className").get_to(className);
                j.at("classDesc").get_to(classDesc);
                j.at("hp").get_to(hp);
                j.at("mp").get_to(mp);
                j.at("str").get_to(str);
                j.at("dex").get_to(dex);
                j.at("int").get_to(_int);
                j.at("wis").get_to(wis);
                j.at("cha").get_to(cha);
                j.at("def").get_to(def);
                j.at("spd").get_to(spd);
                j.at("hpMultiplier").get_to(hpMultiplier);
                j.at("mpMultiplier").get_to(mpMultiplier);
                j.at("strMultiplier").get_to(strMultiplier);
                j.at("dexMultiplier").get_to(dexMultiplier);
                j.at("intMultiplier").get_to(intMultiplier);
                j.at("wisMultiplier").get_to(wisMultiplier);
                j.at("chaMultiplier").get_to(chaMultiplier);
                j.at("defMultiplier").get_to(defMultiplier);
                j.at("spdMultiplier").get_to(spdMultiplier);

                if(j.contains("classId")) j.at("classId").get_to(classId);
                if(j.contains("skills")) j.at("skills").get_to(skills);
            }
    };
}