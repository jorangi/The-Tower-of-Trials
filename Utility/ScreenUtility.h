#pragma once
#include <windows.h>
#include <string>

namespace TTOT::Utilities::ScreenUtility
{
    void Clear();
    void Print(const std::string& text);
    void PrintLine(const std::string& text);
    void HideCursor();
    void ShowCursor();
    void Resize(int cols, int rows);
}