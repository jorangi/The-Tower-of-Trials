#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <memory>
#include "Class/ClassBase.h"
#include <nlohmann/json.hpp>

namespace TTOT::Datas
{
    class EntityDTO
    {
        private:
            uint32_t id;
            std::string name;
            uint32_t hp; // 체력
            uint32_t mp; // 마력
            uint32_t money;
            uint32_t str; // 힘
            uint32_t dex; // 민첩
            uint32_t _int; // 지력
            uint32_t wis; // 지혜
            uint32_t cha; // 매력
            uint32_t def; // 방어력
            uint32_t spd; // 속도
            std::vector<uint32_t> skills;
            std::unique_ptr<TTOT::Class::ClassBase> classInfo;

        public:
            EntityDTO(uint32_t id, const std::string& name, uint32_t hp, uint32_t mp, uint32_t money,
                uint32_t str, uint32_t dex, uint32_t _int, uint32_t wis, uint32_t cha, uint32_t def, uint32_t spd, 
                std::vector<uint32_t> skills, 
                std::unique_ptr<TTOT::Class::ClassBase> classInfo):
                id(id),name(name), hp(hp), mp(mp), money(money), 
                str(str), dex(dex), _int(_int), wis(wis), cha(cha), def(def), spd(spd), 
                skills(skills){this->classInfo = std::move(classInfo);}

            uint32_t GetId() const {return id;}
            std::string GetName() const {return name;}
            uint32_t GetHP() const {return hp;}
            uint32_t GetMP() const {return mp;}
            uint32_t GetMoney() const {return money;}
            uint32_t GetStr() const {return str;}
            uint32_t GetDex() const {return dex;}
            uint32_t GetInt() const {return _int;}
            uint32_t GetWis() const {return wis;}
            uint32_t GetCha() const {return cha;}
            uint32_t GetDef() const {return def;}
            uint32_t GetSpd() const {return spd;}
            std::vector<uint32_t> GetSkills() const {return skills;}
            TTOT::Class::ClassBase* GetClassInfo() const {return classInfo.get();}

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(EntityDTO, id, name, hp, mp, money, str, dex, _int, wis, cha, def, spd, skills, classInfo)

            std::string ToString() const
            {
                std::ostringstream oss;
                oss << "┌──────────────────────────────────────┐" << std::endl;
                oss << "   id: " << id << std::endl;
                oss << "   name: " << name << std::endl;
                oss << " ======================================" << std::endl;
                oss << "   hp: " << hp << std::endl;
                oss << "   mp: " << mp << std::endl;
                oss << "   str: " << str << std::endl;
                oss << "   dex: " << dex << std::endl;
                oss << "   int: " << _int << std::endl;
                oss << "   wis: " << wis << std::endl;
                oss << "   cha: " << cha << std::endl;
                oss << "   def: " << def << std::endl;
                oss << "   spd: " << spd << std::endl;
                oss << " ======================================" << std::endl;
                oss << "   money: " << money << std::endl;
                oss << " ======================================" << std::endl;
                oss << "   skills: ";
                for (auto skill : skills)
                {
                    oss << skill << " ";
                }
                oss << " ======================================" << std::endl;
                oss << " Class: " << classInfo->ToString() << std::endl;
                oss << std::endl;
                oss << "└──────────────────────────────────────┘" << std::endl;

                return oss.str();
            }
    };
    class EntityDTOBuilder
    {
        public:
            EntityDTOBuilder& Id(uint32_t id)
            {
                this->id = id;
                return *this;
            }
            EntityDTOBuilder& Name(const std::string& name)
            {
                this->name = name;
                return *this;
            }
            EntityDTOBuilder& Hp(uint32_t hp)
            {
                this->hp = hp;
                return *this;
            }
            EntityDTOBuilder& Mp(uint32_t mp)
            {
                this->mp = mp;
                return *this;
            }
            EntityDTOBuilder& Money(uint32_t money)
            {
                this->money = money;
                return *this;
            }
            EntityDTOBuilder& Atk(uint32_t str)
            {
                this->str = str;
                return *this;
            }
            EntityDTOBuilder& Dex(uint32_t dex)
            {
                this->dex = dex;
                return *this;
            }
            EntityDTOBuilder& Int(uint32_t _int)
            {
                this->_int = _int;
                return *this;
            }
            EntityDTOBuilder& Wis(uint32_t wis)
            {
                this->wis = wis;
                return *this;
            }
            EntityDTOBuilder& Cha(uint32_t cha)
            {
                this->cha = cha;
                return *this;
            }
            EntityDTOBuilder& Def(uint32_t def)
            {
                this->def = def;
                return *this;
            }
            EntityDTOBuilder& Spd(uint32_t spd)
            {
                this->spd = spd;
                return *this;
            }
            EntityDTOBuilder& Skills(std::vector<uint32_t> skills)
            {
                this->skills = skills;
                return *this;
            }
            EntityDTOBuilder& ClassInfo(std::unique_ptr<TTOT::Class::ClassBase> classInfo)
            {
                this->classInfo = std::move(classInfo);
                return *this;
            }
            EntityDTO Build()
            {
                return EntityDTO(id, name, hp, mp, money, str, dex, _int, wis, cha, def, spd, skills, std::move(classInfo));
            }
    
        private:
            uint32_t id;
            std::string name;
            uint32_t hp;
            uint32_t mp;
            uint32_t money;
            uint32_t str;
            uint32_t dex;
            uint32_t _int;
            uint32_t wis;
            uint32_t cha;
            uint32_t def;
            uint32_t spd;
            std::vector<uint32_t> skills;
            std::unique_ptr<TTOT::Class::ClassBase> classInfo;
    };
}