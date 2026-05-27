#pragma once
#include <windows.h>
#include <iostream>
#include <string>

namespace TTOT::Utility
{
    class ScreenUtility
    {
        public:
            static void Clear();
            static void Print(const std::string& text);
            static void PrintLine(const std::string& text);
            static void HideCursor();
    };
}