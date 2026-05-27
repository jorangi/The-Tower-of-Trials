#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <conio.h>

using json = nlohmann::json;

namespace TTOT::Network{
    class ConnGemini {
        private:
            static std::string CheckAndInitializeApiKey();
        public:
            static std::string Request(const json& body);
            static std::string Request(const std::string& prompt);
    };
}