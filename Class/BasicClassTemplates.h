#pragma once
#include <nlohmann/json.hpp>
#include <vector>

namespace TTOT::Class
{
    inline const std::vector<nlohmann::json> BASIC_CLASS_TEMPLATES = {
        {
            {"className", "전사"},
            {"classDesc", "단단한 신체와 강력한 힘으로 전선의 선봉에 서는 자."},
            {"hp", 5}, {"mp", 1}, {"str", 4}, {"dex", 2}, {"int", 1}, {"wis", 1}, {"cha", 2}, {"def", 1}, {"spd", 1},
            {"hpMultiplier", 1.30}, {"mpMultiplier", 0.70}, {"strMultiplier", 1.30}, {"dexMultiplier", 1.00}, 
            {"intMultiplier", 0.60}, {"wisMultiplier", 0.70}, {"chaMultiplier", 1.00}, {"defMultiplier", 1.30}, {"spdMultiplier", 0.90}
        },
        {
            {"className", "도적"},
            {"classDesc", "어둠 속에 숨어 적의 허점을 찌르고 유유히 사라지는 암살자."},
            {"hp", 3}, {"mp", 1}, {"str", 2}, {"dex", 4}, {"int", 1}, {"wis", 1}, {"cha", 2}, {"def", 1}, {"spd", 3},
            {"hpMultiplier", 0.90}, {"mpMultiplier", 0.80}, {"strMultiplier", 1.10}, {"dexMultiplier", 1.40}, 
            {"intMultiplier", 0.80}, {"wisMultiplier", 0.80}, {"chaMultiplier", 1.00}, {"defMultiplier", 0.80}, {"spdMultiplier", 1.40}
        },
        {
            {"className", "궁수"},
            {"classDesc", "거리를 유지한 채 치명적인 화살로 적을 숨막히게 하는 저격수."},
            {"hp", 3}, {"mp", 2}, {"str", 2}, {"dex", 4}, {"int", 1}, {"wis", 2}, {"cha", 1}, {"def", 1}, {"spd", 2},
            {"hpMultiplier", 1.00}, {"mpMultiplier", 0.90}, {"strMultiplier", 1.10}, {"dexMultiplier", 1.35}, 
            {"intMultiplier", 0.80}, {"wisMultiplier", 1.00}, {"chaMultiplier", 0.90}, {"defMultiplier", 0.90}, {"spdMultiplier", 1.25}
        },
        {
            {"className", "마법사"},
            {"classDesc", "세계의 원소를 다루어 광역 파괴를 불러오는 유리 대포."},
            {"hp", 2}, {"mp", 5}, {"str", 1}, {"dex", 1}, {"int", 4}, {"wis", 3}, {"cha", 1}, {"def", 0}, {"spd", 1},
            {"hpMultiplier", 0.70}, {"mpMultiplier", 1.40}, {"strMultiplier", 0.60}, {"dexMultiplier", 0.90}, 
            {"intMultiplier", 1.45}, {"wisMultiplier", 1.25}, {"chaMultiplier", 1.00}, {"defMultiplier", 0.60}, {"spdMultiplier", 0.90}
        },

        {
            {"className", "성직자"},
            {"classDesc", "신성한 힘으로 아군을 치유하고 불사의 존재를 정화하는 인도자."},
            {"hp", 3}, {"mp", 3}, {"str", 2}, {"dex", 1}, {"int", 2}, {"wis", 4}, {"cha", 1}, {"def", 1}, {"spd", 1},
            {"hpMultiplier", 1.10}, {"mpMultiplier", 1.20}, {"strMultiplier", 1.00}, {"dexMultiplier", 0.80}, 
            {"intMultiplier", 1.10}, {"wisMultiplier", 1.35}, {"chaMultiplier", 1.10}, {"defMultiplier", 1.05}, {"spdMultiplier", 0.85}
        }
    };
}