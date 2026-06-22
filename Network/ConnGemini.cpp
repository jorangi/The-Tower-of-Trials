#include "Network/ConnGemini.h"
#include <iostream>
#include <string>
#include <windows.h>

namespace TTOT::Network
{
    ConnGemini::ConnGemini(){
        if(!apiKey.empty()) return;
        apiKey = CheckAndInitializeApiKey();
    }
    std::string ConnGemini::CheckAndInitializeApiKey()
    {
        const char* envKey = std::getenv("GEMINI_API_KEY");
        if (envKey == nullptr)
        {
            std::cout << "현재 환경에서 Gemini API를 찾을 수 없습니다." << std::endl;
            std::cout << "Gemini API 키를 입력하세요 : " << std::endl;
            std::string inputKey;
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode = 0;
            GetConsoleMode(hStdin, &mode);
            SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
            std::getline(std::cin, inputKey);
            SetConsoleMode(hStdin, mode);
            std::cout << std::endl;
            if(inputKey.empty())
            {
                std::cout << "❌ 올바르지 않은 API 키 형식입니다. 프로그램이 정상 작동하지 않을 수 있습니다." << std::endl;
            }
            _putenv_s("GEMINI_API_KEY", inputKey.c_str());
            std::cout << "✅ API 키가 임시로 세팅되었습니다! (프로그램 종료 시 소멸)" << std::endl;
            return inputKey;
        }
        else
        {
            std::cout << "[System] 윈도우 시스템 환경 변수에서 GEMINI_API_KEY를 성공적으로 감지했습니다." << std::endl << std::endl;
            return std::string(envKey);
        }
    }
    std::string ConnGemini::Request(const json& body)
    {
        std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-3.1-flash-lite:generateContent?key=" + apiKey;

        int maxRetries = 3;
        int delayMs = 1000;

        for (int attempt = 0; attempt < maxRetries; attempt++)
        {
            cpr::Response response = cpr::Post(
                cpr::Url{url},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{body.dump()}
            );

            if (response.status_code == 200)
            {
                auto responseJson = json::parse(response.text);
                return responseJson["candidates"][0]["content"]["parts"][0]["text"];
            }
            else if (response.status_code == 503 && attempt < maxRetries - 1)
            {
                Sleep(delayMs);
                delayMs *= 2;
                continue;
            }
            else
            {
                return "Error: " + std::to_string(response.status_code) + " - " + response.text;
            }
        }
        return "Error: 만료된 요청";
    }
    std::string ConnGemini::Request(const std::string& prompt)
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
        return Request(body);
    }
}