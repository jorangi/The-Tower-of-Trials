#include "Entity/PlayerFactory.h"
#include "Component/HealthComponent.h"
#include "Component/ManaComponent.h"
#include "Component/MoneyComponent.h"
#include "Component/StatComponent.h"
#include "Component/ClassComponent.h"

namespace TTOT::Entities
{
    std::unique_ptr<TTOT::Entities::Player> TTOT::Entities::PlayerFactory::CreatePlayer(TTOT::Datas::EntityDTO&& dto, const bool gender)
    {
        auto player = std::make_unique<Player>(dto.GetId(), dto.GetName(), gender);
        player->AddComponent(std::make_unique<Components::HealthComponent>(dto.GetHP()));
        player->AddComponent(std::make_unique<Components::ManaComponent>(dto.GetMP()));
        player->AddComponent(std::make_unique<Components::MoneyComponent>(dto.GetMoney()));
        auto stat = std::make_unique<Components::StatComponentBuilder>();
        stat->SetSTR(dto.GetStr());
        stat->SetDEX(dto.GetDex());
        stat->SetINT(dto.GetInt());
        stat->SetWIS(dto.GetWis());
        stat->SetCHA(dto.GetCha());
        stat->SetDEF(dto.GetDef());
        stat->SetSPD(dto.GetSpd());
        player->AddComponent(std::move(stat->Build()));
        auto movedClass = dto.ReleaseClassInfo();
        if (movedClass != nullptr) 
        {
            player->AddComponent(std::make_unique<Components::ClassComponent>(std::move(movedClass)));
        }
        return std::move(player);
    }
}