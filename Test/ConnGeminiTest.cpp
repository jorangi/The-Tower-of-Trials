#include "Network/ConnGemini.h"
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;
using namespace TTOT::Network;

void TestWithBody(const string& prompt)
{
    json body = {
        {"contents", json::array({
            {
                {"parts", json::array({
                    {{"text", prompt}}
                })}
            }
        })}
    };
    ConnGemini* conn = new ConnGemini();
    string response = conn->Request(body);
    cout << "응답: " << response << endl;
    cout << response << endl;
}
void TestOnlyPrompt(const string& prompt)
{
    ConnGemini* conn = new ConnGemini();
    string response = conn->Request(prompt);
    cout << "응답: " << response << endl;
    cout << response << endl;
}

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    cout << "===============================" << endl;
    cout << "   Body 포함 프롬프트 테스트       " << endl;
    cout << "===============================" << endl;
    string prompt;
    cout << "프롬프트를 입력하세요 : ";
    std::getline(std::cin, prompt);
    TestWithBody(prompt);
    cout << "===============================" << endl;
    cout << "        프롬프트만 테스트       " << endl;
    cout << "===============================" << endl;
    cout << "프롬프트를 입력하세요 : ";
    std::getline(std::cin, prompt);
    TestOnlyPrompt(prompt);
}