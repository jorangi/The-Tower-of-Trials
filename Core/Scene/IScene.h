#pragma once
#include <string>

namespace TTOT::Core::Scenes
{
    class IScene
    {
        public:
            virtual ~IScene() = default;
            /// @brief 씬에 진입할 때
            virtual void OnEnter() = 0;
            /// @brief 씬에서 탈출할 때
            virtual void OnExit() = 0;
            /// @brief 씬 업데이트
            virtual void Update() = 0;
            /// @brief 입력 처리
            /// @param input 입력 내용
            virtual void HandleInput(const std::string& input) = 0;
            /// @brief 입력 오류 처리
            /// @param errorMessage 오류 메시지
            virtual void InputError(const std::string& errorMessage) = 0;
    };
}