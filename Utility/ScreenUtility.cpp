#include "ScreenUtility.h"

namespace TTOT::Utility
{
    void ScreenUtility::Clear()
    {
        system("cls");
    }

    void ScreenUtility::Print(const std::string& text)
    {
        std::cout << text;
    }
    void ScreenUtility::PrintLine(const std::string& text)
    {
        std::cout << text << std::endl;
    }
    void ScreenUtility::HideCursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    }
}