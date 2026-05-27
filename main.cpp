#include <iostream>
#include <windows.h>

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "main.cpp 실행됨" << std::endl;
    return 0;
}