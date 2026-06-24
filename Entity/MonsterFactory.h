#pragma once
#include "Component/StatComponent.h"
#include "Datas/EntityDTO.h"
#include "Monster.h"

namespace TTOT::Entities {
class MonsterFactory {
public:
  static unique_ptr<Monster> CreateMonster(uint32_t id,
                                           const TTOT::Datas::EntityDTO &dto) {
    auto monster = std::make_unique<Monster>(id, dto.GetName());

    auto health = std::make_unique<Components::HealthComponent>(dto.GetHP());
    auto mana = std::make_unique<Components::HealthComponent>(dto.GetHP());
    auto money = std::make_unique<Components::MoneyComponent>(dto.GetMoney());
    auto stat = std::make_unique<Components::StatComponentBuilder>();
    stat->SetSTR(dto.GetStr());
    stat->SetDEX(dto.GetDex());
    stat->SetINT(dto.GetInt());
    stat->SetWIS(dto.GetWis());
    stat->SetCHA(dto.GetCha());
    stat->SetDEF(dto.GetDef());
    stat->SetSPD(dto.GetSpd());

    monster->AddComponent(std::move(health));
    monster->AddComponent(std::move(mana));
    monster->AddComponent(std::move(money));
    monster->AddComponent(std::move(stat->Build()));

    return monster;
  }
};
} // namespace TTOT::Entities