#include <iostream>
#include <string>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>

namespace TTOT::Datas
{
    class EntityDTO
    {
        private:
            uint32_t id;
            std::string name;
            uint32_t hp;
            uint32_t mp;
            uint32_t money;
            uint32_t atk;
            uint32_t def;
            uint32_t spd;
            std::vector<uint32_t> skills;

        public:
            EntityDTO(uint32_t id, const std::string& name, uint32_t hp, uint32_t mp, uint32_t money, uint32_t atk, uint32_t def, uint32_t spd, std::vector<uint32_t> skills):
                id(id), name(name), hp(hp), mp(mp), money(money), atk(atk), def(def), spd(spd), skills(skills){}
            uint32_t GetId() const {return id;}
            std::string GetName() const {return name;}
            uint32_t GetHP() const {return hp;}
            uint32_t GetMP() const {return mp;}
            uint32_t GetMoney() const {return money;}
            uint32_t GetAtk() const {return atk;}
            uint32_t GetDef() const {return def;}
            uint32_t GetSpd() const {return spd;}
            std::vector<uint32_t> GetSkills() const {return skills;}
            NLOHMANN_DEFINE_TYPE_INTRUSIVE(EntityDTO, id, name, hp, mp, money, atk, def, spd, skills)

            std::string ToString() const
            {
                std::ostringstream oss;
                oss << "┌──────────────────────────────────────┐" << std::endl;
                oss << "   id: " << id << std::endl;
                oss << "   name: " << name << std::endl;
                oss << " ======================================" << std::endl;
                oss << "   hp: " << hp << std::endl;
                oss << "   mp: " << mp << std::endl;
                oss << "   atk: " << atk << std::endl;
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
                oss << std::endl;
                oss << "└──────────────────────────────────────┘" << std::endl;

                return oss.str();
            }
    };
    class EntityDTOBuilder
    {
        public:
            EntityDTOBuilder Id(uint32_t id)
            {
                this->id = id;
                return *this;
            }
            EntityDTOBuilder Name(const std::string& name)
            {
                this->name = name;
                return *this;
            }
            EntityDTOBuilder Hp(uint32_t hp)
            {
                this->hp = hp;
                return *this;
            }
            EntityDTOBuilder Mp(uint32_t mp)
            {
                this->mp = mp;
                return *this;
            }
            EntityDTOBuilder Money(uint32_t money)
            {
                this->money = money;
                return *this;
            }
            EntityDTOBuilder Atk(uint32_t atk)
            {
                this->atk = atk;
                return *this;
            }
            EntityDTOBuilder Def(uint32_t def)
            {
                this->def = def;
                return *this;
            }
            EntityDTOBuilder Spd(uint32_t spd)
            {
                this->spd = spd;
                return *this;
            }
            EntityDTOBuilder Skills(std::vector<uint32_t> skills)
            {
                this->skills = skills;
                return *this;
            }
            EntityDTO Build()
            {
                return EntityDTO(id, name, hp, mp, money, atk, def, spd, skills);
            }
    
        private:
            uint32_t id;
            std::string name;
            uint32_t hp;
            uint32_t mp;
            uint32_t money;
            uint32_t atk;
            uint32_t def;
            uint32_t spd;
            std::vector<uint32_t> skills;
    };
}