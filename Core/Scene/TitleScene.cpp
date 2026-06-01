#include "Core/Scene/TitleScene.h"
#include <Utility/ScreenUtility.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <windows.h>

namespace TTOT::Core::Scenes
{
    TTOT::Core::Scenes::TitleScene::TitleScene(TTOT::Core::GameContext& context) : context(context){}
    void TTOT::Core::Scenes::TitleScene::OnEnter()
    {
        std::cout << "┌─────────────────────────────────────────────┐" << std::endl;
        std::cout << "│             The Tower of Trials             │" << std::endl;
        std::cout << "└─────────────────────────────────────────────┘" << std::endl;
        std::cout << "\n\n\n" << std::endl;
        std::cout << "                 1. 불러오기" << std::endl;
        std::cout << "                 2. 새 게임" << std::endl;
        std::cout << "                 3. 설정" << std::endl;
        std::cout << "                 4. 종료" << std::endl;
    }
    void TTOT::Core::Scenes::TitleScene::OnExit(){}
    void TTOT::Core::Scenes::TitleScene::Update(){}
    void TTOT::Core::Scenes::TitleScene::HandleInput(const std::string& input)
    {
        std::string clearInput = input;
        clearInput.erase(
            std::remove_if(
                clearInput.begin(), clearInput.end(), ::isspace), clearInput.end()
            );
        if(clearInput == "1" || clearInput == "불러오기" || clearInput == "1.불러오기")
        {
            std::cout << "불러오기 선택됨" << std::endl;
        }
        else if(clearInput == "2" || clearInput == "새게임" || clearInput == "2.새게임")
        {
            std::cout << "새 게임 선택됨" << std::endl;
        }
        else if(clearInput == "3" || clearInput == "설정" || clearInput == "3.설정")
        {
            std::cout << "설정 선택됨" << std::endl;
        }
        else if(clearInput == "4" || clearInput == "종료" || clearInput == "4.종료")
        {
            std::cout << "종료 선택됨" << std::endl;
            exit(0);
        }
        else
        {
            InputError("잘못된 입력입니다.");
        }
        Sleep(1000);
        TTOT::Utilities::ScreenUtility::Clear();
        OnEnter();
    }
    void TTOT::Core::Scenes::TitleScene::InputError(const std::string& errorMessage)
    {
        std::cout << "입력 오류: " << errorMessage << std::endl;
    }
}