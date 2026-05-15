#include <iostream>
#include <windows.h>
#include <Entity/MonsterFactory.h>
#include <cassert>

using namespace std;

void Test_CreateMonster()
{
    TTOT::Datas::EntityDTOBuilder builder = TTOT::Datas::EntityDTOBuilder();
    TTOT::Datas::EntityDTO dto = builder.Id(1)
                            .Name("슬라임")
                            .Hp(30)
                            .Money(20)
                            .Atk(5)
                            .Def(1)
                            .Spd(3)
                            .Build();

    auto monster = TTOT::Entities::MonsterFactory::CreateMonster(dto);
    assert(monster->GetId() == 1);
    assert(monster->GetName() == "슬라임");
    cout << monster->GetComponent<TTOT::Components::HealthComponent>()->ToString() << endl;
    cout << monster->GetComponent<TTOT::Components::MoneyComponent>()->ToString() << endl;
    cout << monster->ToString() << endl;
    cout << "몬스터 팩토리 + 몬스터 생성완료" << endl;

}

// int main()
// {
//     Test_CreateMonster();
// }