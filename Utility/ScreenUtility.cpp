#include "ScreenUtility.h"
#include <iostream>

namespace TTOT::Utilities::ScreenUtility
{
    void Clear()
    {
        system("cls");
    }
    void Print(const std::string& text)
    {
        std::cout << text;
    }
    void PrintLine(const std::string& text)
    {
        std::cout << text << std::endl;
    }
    void ShowCursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    }
    void HideCursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    }
}