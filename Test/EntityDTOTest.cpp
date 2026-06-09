#include <cassert>
#include <iostream>
#include <windows.h>
#include "Datas/EntityDTO.h"

using namespace std;
using namespace TTOT::Datas;

void Test_CreationMonster()
{
    EntityDTOBuilder builder = EntityDTOBuilder();
    EntityDTO dto = builder.Id(1)
                            .Name("슬라임")
                            .Hp(30)
                            .Money(20)
                            .Str(5)
                            .Def(1)
                            .Spd(3)
                            .Build();

    assert(dto.GetId() == 1);
    assert(dto.GetName() == "슬라임");
    assert(dto.GetHP() == 30);
    assert(dto.GetMP() == 0);
    assert(dto.GetMoney() == 20);
    assert(dto.GetStr() == 5);
    assert(dto.GetDef() == 1);
    assert(dto.GetSpd() == 3);
    SetConsoleOutputCP(CP_UTF8);
    cout << "Entity DTO 생성됨" << endl;
    cout << dto.ToString() << endl;
}
// int main()
// {
//     Test_CreationMonster();
// }