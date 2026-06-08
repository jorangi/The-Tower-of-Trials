#include <windows.h>
#include "Core/GameManager.h"

using namespace std;
using namespace TTOT::Network;
using namespace TTOT::Utilities;

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
    TTOT::Core::GameManager gameManager;
    gameManager.Run();
    return 0;
}