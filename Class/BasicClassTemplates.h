#pragma once
#include <nlohmann/json.hpp>
#include <vector>

namespace TTOT::Class
{
    inline const std::vector<nlohmann::json> BASIC_CLASS_TEMPLATES = {
        {
            {"className", "전사"},
            {"classDesc", "무너진 전장에서 홀로 살아남아 아군을 잃은 죄책감과 더 강력한 힘에 대한 갈망으로 탑의 문을 두드렸습니다. 앞을 가로막는 시련 앞에서도 물러서지 않으며, 강인한 신체와 굳건한 방패로 적의 맹공을 묵묵히 버텨내고 반격의 기회를 노리는 듬직한 전위입니다. 하지만 복잡한 마법이나 기교와는 거리가 멀어 변칙적인 위기 상황에는 둔감하게 반응하곤 합니다. 탑의 거주민들은 이 무모하리만치 단단한 기백을 경외하면서도, 그 고집스러움을 은밀히 이용하려 들 것입니다."},
            {"hp", 5}, {"mp", 1}, {"str", 4}, {"dex", 2}, {"int", 1}, {"wis", 1}, {"cha", 2}, {"def", 1}, {"spd", 1},
            {"hpMultiplier", 1.30}, {"mpMultiplier", 0.70}, {"strMultiplier", 1.30}, {"dexMultiplier", 1.00}, 
            {"intMultiplier", 0.60}, {"wisMultiplier", 0.70}, {"chaMultiplier", 1.00}, {"defMultiplier", 1.30}, {"spdMultiplier", 0.90}
        },
        {
            {"className", "도적"},
            {"classDesc", "지하 세계의 피비린내 나는 숙청에서 도망쳐, 전설의 유물을 훔치거나 과거를 세탁하기 위해 그림자 속에서 탑으로 흘러들었습니다. 찰나의 틈을 놓치지 않는 기민함으로 누구보다 먼저 움직이며, 상대가 숨을 고르기도 전에 치명적인 급소를 꿰뚫는 암살자입니다. 다만 일격에 적의 숨통을 끊지 못해 피를 말리는 진흙탕 싸움으로 이어지거나, 묵직한 공격을 정면에서 허용하면 한순간에 목숨이 위태로워집니다. 상인과 정보상들은 이 자의 날카로운 감각을 신뢰하면서도, 언제 등 뒤에서 단검이 날아올지 몰라 늘 경계합니다."},
            {"hp", 3}, {"mp", 1}, {"str", 2}, {"dex", 4}, {"int", 1}, {"wis", 1}, {"cha", 2}, {"def", 1}, {"spd", 3},
            {"hpMultiplier", 0.90}, {"mpMultiplier", 0.80}, {"strMultiplier", 1.10}, {"dexMultiplier", 1.40}, 
            {"intMultiplier", 0.80}, {"wisMultiplier", 0.80}, {"chaMultiplier", 1.00}, {"defMultiplier", 0.80}, {"spdMultiplier", 1.40}
        },
        {
            {"className", "궁수"},
            {"classDesc", "고향 숲을 불태운 거대한 재앙의 진실을 쫓아, 탑의 정상에 있다는 예언의 눈을 얻기 위해 차가운 활시위를 쥐고 시련의 궤도에 올랐습니다. 상대의 빈틈을 예리하게 포착하여 가차 없는 화살비를 퍼붓는 정밀 타격의 명수입니다. 균형 잡힌 신체를 지녔으나, 맹렬하게 파고드는 적의 공세를 직접 몸으로 받아내야 하는 난전에서는 힘의 열세가 뚜렷하게 드러납니다. 탑의 경비대나 사냥꾼들은 이 자의 냉철한 시야를 높게 평가하며 넌지시 동맹을 제안해오곤 합니다."},
            {"hp", 3}, {"mp", 2}, {"str", 2}, {"dex", 4}, {"int", 1}, {"wis", 2}, {"cha", 1}, {"def", 1}, {"spd", 2},
            {"hpMultiplier", 1.00}, {"mpMultiplier", 0.90}, {"strMultiplier", 1.10}, {"dexMultiplier", 1.35}, 
            {"intMultiplier", 0.80}, {"wisMultiplier", 1.00}, {"chaMultiplier", 0.90}, {"defMultiplier", 0.90}, {"spdMultiplier", 1.25}
        },
        {
            {"className", "마법사"},
            {"classDesc", "이계의 금지된 마법을 탐하다 이성을 잃을 뻔한 위기를 겪고, 자신의 마력 저주를 풀거나 무한한 지식을 독점하기 위해 탑의 흐름을 역행해 들어왔습니다. 폭발적인 원소 마법을 쏟아부어 적을 잿더미로 만드는 압도적인 파괴력의 소유자입니다. 심연을 엿보는 지력과 마나를 자랑하지만 물리적인 육신은 종잇장처럼 연약하여, 찰나의 방심이나 방어 주문의 실패가 곧바로 끔찍한 죽음으로 이어집니다. 학자들은 이 자의 지식에 집착하지만, 평범한 이들은 언제 폭주할지 모르는 위험분자로 취급해 거리를 둡니다."},
            {"hp", 2}, {"mp", 5}, {"str", 1}, {"dex", 1}, {"int", 4}, {"wis", 3}, {"cha", 1}, {"def", 0}, {"spd", 1},
            {"hpMultiplier", 0.70}, {"mpMultiplier", 1.40}, {"strMultiplier", 0.60}, {"dexMultiplier", 0.90}, 
            {"intMultiplier", 1.45}, {"wisMultiplier", 1.25}, {"chaMultiplier", 1.00}, {"defMultiplier", 0.60}, {"spdMultiplier", 0.90}
        },
        {
            {"className", "성직자"},
            {"classDesc", "맹신하던 신전이 타락하는 것을 목도한 후, 진정한 신성 의지를 증명하고 탑 내부의 왜곡된 존재들을 정화하기 위해 성호를 그으며 발을 들였습니다. 상처를 치유하고 가호의 성벽을 두르며, 끈질긴 공방전 속에서 적을 서서히 무너뜨리는 불굴의 구도자입니다. 기묘한 저주나 망령을 정화하는 데 탁월하지만, 발걸음과 기백이 무거워 기민하게 덮쳐오는 적들에게 늘 첫 일격을 허용하고 맙니다. 고통받는 하층민들과 방랑자들은 이 자를 구원의 등불로 우러러보며 곁에 머물고자 합니다."},
            {"hp", 3}, {"mp", 3}, {"str", 2}, {"dex", 1}, {"int", 2}, {"wis", 4}, {"cha", 1}, {"def", 1}, {"spd", 1},
            {"hpMultiplier", 1.10}, {"mpMultiplier", 1.20}, {"strMultiplier", 1.00}, {"dexMultiplier", 0.80}, 
            {"intMultiplier", 1.10}, {"wisMultiplier", 1.35}, {"chaMultiplier", 1.10}, {"defMultiplier", 1.05}, {"spdMultiplier", 0.85}
        }
    };
}