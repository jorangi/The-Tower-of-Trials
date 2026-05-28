#include <windows.h>
#include "Network/ConnGemini.h"
#include "Utility/ScreenUtility.h"

using namespace std;
using namespace TTOT::Network;
using namespace TTOT::Utilities;

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ScreenUtility::HideCursor();
    ConnGemini* conn = new ConnGemini();
    Sleep(1000);
    ScreenUtility::Clear();
    Sleep(5000);
    ScreenUtility::ShowCursor();
    return 0;
}