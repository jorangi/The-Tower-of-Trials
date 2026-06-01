#include <windows.h>
#include "Core/GameManager.h"

using namespace std;
using namespace TTOT::Network;
using namespace TTOT::Utilities;

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    TTOT::Core::GameManager gameManager;
    gameManager.Run();
    return 0;
}