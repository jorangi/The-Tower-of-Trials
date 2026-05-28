#pragma once
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <conio.h>

using json = nlohmann::json;

namespace TTOT::Network{
    class ConnGemini {
        private:
            inline static std::string apiKey;
            /// @brief API 키를 확인하고 초기화하는 함수
            /// @return 초기화된 API 키
            std::string CheckAndInitializeApiKey();
            ConnGemini(const ConnGemini&) = delete;
            ConnGemini& operator=(const ConnGemini&) = delete;

        public:
            /// @brief ConnGemini 클래스 생성자, API 키를 확인 및 초기화
            ConnGemini();
            /// @brief Gemini API에 요청을 보내는 함수
            /// @param body 요청 본문
            /// @return API 응답
            std::string Request(const json& body);
            /// @brief Gemini API에 프롬프트를 보내는 함수
            /// @param prompt 요청 프롬프트
            /// @return API 응답
            std::string Request(const std::string& prompt);
    };
}