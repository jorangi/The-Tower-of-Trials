#include <windows.h>
#include "Entity/Player.h"
#include <cassert>

using namespace TTOT::Entities;

void Player_Creation()
{
    Player player(1, "신종혁");
    assert(player.GetId() == 1);
    assert(player.GetName() == "신종혁");
    cout << "플레이어 생성 테스트 통과" << endl;
    
    player.ShowInfo();
}

// int main(void)
// {
//     SetConsoleOutputCP(CP_UTF8);
//     Player_Creation();
// }