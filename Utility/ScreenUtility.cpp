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
    void Resize(int cols, int rows)
    {
        // 2. Windows API Check (avoid shaking if size matches)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) return;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            int currentCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int currentRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            if (currentCols == cols && currentRows == rows) {
                return; // Already the correct size, skip!
            }
        }

        // 1. ANSI escape sequence (Windows Terminal, VS Code, Unix)
        std::cout << "\033[8;" << rows << ";" << cols << "t" << std::flush;

        // 2. Windows API Fallback (Legacy cmd.exe)
        COORD bufferSize;
        bufferSize.X = cols;
        bufferSize.Y = rows;

        SMALL_RECT windowSize;
        windowSize.Left = 0;
        windowSize.Top = 0;
        windowSize.Right = cols - 1;
        windowSize.Bottom = rows - 1;

        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            COORD newBuf = csbi.dwSize;
            bool changeBuffer = false;
            if (cols > newBuf.X) { newBuf.X = cols; changeBuffer = true; }
            if (rows > newBuf.Y) { newBuf.Y = rows; changeBuffer = true; }
            if (changeBuffer) {
                SetConsoleScreenBufferSize(hConsole, newBuf);
            }
            SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
            SetConsoleScreenBufferSize(hConsole, bufferSize);
        }
    }
}