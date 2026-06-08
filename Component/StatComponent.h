#pragma once
#include <ostream>
#include <iostream>
#include <sstream>
#include "Component/Components.h"
#include "Stat/ModifiableStat.h"

namespace TTOT::Components
{
    class StatComponent;
    class StatComponentBuilder
    {
        private:
            uint32_t str;
            uint32_t dex;
            uint32_t _int;
            uint32_t wis;
            uint32_t cha;
            uint32_t def;
            uint32_t spd;
        public:
            StatComponentBuilder() = default;
            StatComponentBuilder& SetSTR(uint32_t val){ this->str = val; return *this; }
            StatComponentBuilder& SetDEX(uint32_t val){ this->dex = val; return *this; }
            StatComponentBuilder& SetINT(uint32_t val){ this->_int = val; return *this; }
            StatComponentBuilder& SetWIS(uint32_t val){ this->wis = val; return *this; }
            StatComponentBuilder& SetCHA(uint32_t val){ this->cha = val; return *this; }
            StatComponentBuilder& SetDEF(uint32_t val){ this->def = val; return *this; }
            StatComponentBuilder& SetSPD(uint32_t val){ this->spd = val; return *this; }
            std::unique_ptr<StatComponent> Build();
    };

    class StatComponent : public Component
    {
        private:
            friend std::unique_ptr<StatComponent> StatComponentBuilder::Build();
            friend class StatComponentBuilder;
            TTOT::Stats::ModifiableStat str;
            TTOT::Stats::ModifiableStat dex;
            TTOT::Stats::ModifiableStat _int;
            TTOT::Stats::ModifiableStat wis;
            TTOT::Stats::ModifiableStat cha;
            TTOT::Stats::ModifiableStat def;
            TTOT::Stats::ModifiableStat spd;

            
            /// @brief 초기 인자 삽입
            /// @param str 힘
            /// @param dex 민첩
            /// @param _int 지능
            /// @param wis 지혜
            /// @param cha 매력
            /// @param def 방어력
            /// @param spd 속도
            StatComponent(uint32_t str, uint32_t dex, uint32_t _int, uint32_t wis, uint32_t cha, uint32_t def, uint32_t spd): str(str, "STR"), dex(dex, "DEX"), _int(_int, "INT"), wis(wis, "WIS"), cha(cha, "CHA"), def(def, "DEF"), spd(spd, "SPD"){}

        public:
            uint32_t GetSTR() const {return str.GetValue();}
            uint32_t GetDEX() const {return dex.GetValue();}
            uint32_t GetINT() const {return _int.GetValue();}
            uint32_t GetWIS() const {return wis.GetValue();}
            uint32_t GetCHA() const {return cha.GetValue();}
            uint32_t GetDEF() const {return def.GetValue();}
            uint32_t GetSPD() const {return spd.GetValue();}
            uint32_t GetTypeID() const override {return GetComponentTypeID<StatComponent>();}
            std::string ToString() const override
            {
                ostringstream oss;
                oss << "STR: " << str.GetValue() << endl;
                oss << "DEX: " << dex.GetValue() << endl;
                oss << "INT: " << _int.GetValue() << endl;
                oss << "WIS: " << wis.GetValue() << endl;
                oss << "CHA: " << cha.GetValue() << endl;
                oss << "DEF: " << def.GetValue() << endl;
                oss << "SPD: " << spd.GetValue() << endl;

                return oss.str();
            }
    };
    inline std::unique_ptr<StatComponent> StatComponentBuilder::Build()
    {
        return std::unique_ptr<StatComponent>(
            new StatComponent(str, dex, _int, wis, cha, def, spd)
        );
    }
}