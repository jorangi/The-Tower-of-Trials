#pragma once
#include "Utility/miniaudio.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace TTOT::Utilities
{
    /// @brief SoundManager 클래스, miniaudio 라이브러리를 사용하여 사운드 파일을 관리하고 재생하는 싱글톤 클래스
    /// @details miniaudio 엔진을 기반으로 동작하며, Assets 폴더의 사운드를 자동 로드합니다.
    class SoundManager
    {
        private:
            ma_engine s_engine;
            std::unordered_map<std::string, std::unique_ptr<ma_sound>> bgmMap;
            std::unordered_map<std::string, std::vector<std::unique_ptr<ma_sound>>> sfxPoolMap;
            ma_sound* currentBGM = nullptr;

            float masterVolume = 1.0f;
            float bgmVolume = 1.0f;
            float sfxVolume = 1.0f;
            
            SoundManager(const SoundManager&) = delete;
            void operator=(const SoundManager&) = delete;
        public:
            /// @brief 사운드 매니저 클래스 생성자, Assets 폴더 내의 모든 사운드 파일을 자동으로 등록
            SoundManager();
            /// @brief 사운드 매니저 소멸자, 등록된 모든 사운드와 엔진 리소스 해제
            ~SoundManager();
            /// @brief 사운드 등록 함수, 파일 경로에서 사운드를 로드하여 키와 함께 저장
            void RegisterSound(const std::string& key, const std::string& filePath, bool isLoop = false);
            /// @brief BGM 재생 함수, 키에 해당하는 사운드를 배경음악으로 재생
            void PlayBGM(const std::string& key);
            /// @brief BGM 정지 함수, 현재 재생 중인 배경음악을 멈추고 위치를 초기화
            void StopBGM();
            /// @brief SFX 재생 함수, 키에 해당하는 사운드를 효과음으로 재생, 이미 재생 중인 경우 위치를 초기화하여 다시 재생
            /// @param key 재생할 효과음의 키값 (예: "SFX_Attack")
            void PlaySFX(const std::string& key);
            /// @brief 볼륨 설정 함수, 마스터 볼륨과 BGM/SFX 볼륨을 조절하여 전체 사운드 출력 레벨을 관리
            /// @param volume 설정할 볼륨 값 (0.0f ~ 1.0f)
            void SetMasterVolume(float volume);
            /// @brief BGM 볼륨 설정 함수, 현재 재생 중인 배경음악의 볼륨을 조절
            /// @param volume 설정할 볼륨 값 (0.0f ~ 1.0f)
            void SetBGMVolume(float volume);
            /// @brief SFX 볼륨 설정 함수, 효과음의 볼륨을 조절
            /// @param volume 설정할 볼륨 값 (0.0f ~ 1.0f)
            void SetSFXVolume(float volume);
    };
}