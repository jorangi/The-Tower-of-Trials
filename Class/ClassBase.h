#pragma once
#include "Class/IClass.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace TTOT::Class
{
    class ClassBase : public IClass
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
            //

        public:
            virtual std::string ToString() const override{return className + " : " + classDesc;}
            std::string GetClassName() const {return className;};
            std::string GetClassDesc() const {return classDesc;};
            friend void from_json(const nlohmann::json& j, ClassBase& c)
            {
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
            }
    };
}